#include "AcquisitionThread.h"

#include "UDPConnection.h"
#include "VelodynePacket.h"
#include "PacketsBuffer.h"

#include <stdlib.h>

using namespace boost;
using namespace std;

AcquisitionThread::AcquisitionThread() throw(ThreadException) {
  bRun = false;
  mThreadHandle = (pthread_t*)malloc(sizeof(pthread_t));
  if (mThreadHandle == NULL) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: malloc failed");
  }
  if (pthread_attr_init(&mThreadAttr)){
    throw ThreadException("AcquisitionThread::AcquisitionThread: init thread attributes failed");
  }
  if (pthread_attr_setdetachstate(&mThreadAttr, PTHREAD_CREATE_JOINABLE)) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: thread set in joinable state failed");
  }
  if (pthread_attr_setschedpolicy(&mThreadAttr, SCHED_RR)) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: thread set scheduling policy failed");
  }
  struct sched_param schedulingParam;
  schedulingParam.sched_priority = (sched_get_priority_max(SCHED_RR) -
                                    sched_get_priority_min(SCHED_RR)) / 2;
  if (pthread_attr_setschedparam(&mThreadAttr, &schedulingParam)) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: thread set scheduling param failed");
  }
  if (pthread_mutex_init(&mMutex, NULL)) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: mutex init failed");
  }
  if (pthread_cond_init(&mCondition, NULL)) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: condition init failed");
  }
  if (pthread_create(mThreadHandle, &mThreadAttr, threadFunction, this)) {
    throw ThreadException("AcquisitionThread::AcquisitionThread: thread create failed");
  }
}

AcquisitionThread::AcquisitionThread(const AcquisitionThread &other) {
}

AcquisitionThread& AcquisitionThread::operator =
  (const AcquisitionThread &other) {
}

AcquisitionThread::~AcquisitionThread() throw(ThreadException) {
  run();
  pthread_cancel(*mThreadHandle);
  if (pthread_join(*mThreadHandle, NULL)) {
    throw ThreadException("AcquisitionThread::~AcquisitionThread: join thread failed");
  }
  free(mThreadHandle);
  mThreadHandle = NULL;
  if (pthread_attr_destroy(&mThreadAttr)) {
    throw ThreadException("AcquisitionThread::~AcquisitionThread: thread attributes destroy failed");
  }
  if (pthread_mutex_destroy(&mMutex)) {
    throw ThreadException("AcquisitionThread::~AcquisitionThread: mutex destroy failed");
  }
  if (pthread_cond_destroy(&mCondition)) {
    throw ThreadException("AcquisitionThread::~AcquisitionThread: condition destroy failed");
  }
}

void* AcquisitionThread::threadFunction(void *arg)
  throw(ThreadException, IOException) {
  AcquisitionThread *classHandle = (AcquisitionThread*)arg;
  if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
    throw ThreadException("AcquisitionThread::threadFunction: set cancel state failed");
  }
  if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
    throw ThreadException("AcquisitionThread::threadFunction: set cancel type failed");
  }
  pthread_cleanup_push(threadCleanupFunction, NULL);
  UDPConnection com;
  com.open();
  while (1) {
    pthread_mutex_lock(&classHandle->mMutex);
    if (classHandle->bRun == false) {
      pthread_cond_wait(&(classHandle->mCondition), &(classHandle->mMutex));
      pthread_testcancel();
    }
    pthread_mutex_unlock(&(classHandle->mMutex));
    shared_ptr<VelodynePacket> vdynePacket(new VelodynePacket());
    com >> *vdynePacket;
    classHandle->mPacketsBuffer.pushPacket(vdynePacket);
    pthread_testcancel();
  }
  com.close();
  pthread_cleanup_pop(0);
}

void AcquisitionThread::run() {
  pthread_mutex_lock(&mMutex);
  bRun = true;
  pthread_cond_signal(&mCondition);
  pthread_mutex_unlock(&mMutex);
}

void AcquisitionThread::stop() {
  pthread_mutex_lock(&mMutex);
  bRun = false;
  pthread_mutex_unlock(&mMutex);
}

void AcquisitionThread::threadCleanupFunction(void *arg) {
}

shared_ptr<VelodynePacket> AcquisitionThread::getPacket() throw(IOException) {
  return mPacketsBuffer.popPacket();
}
