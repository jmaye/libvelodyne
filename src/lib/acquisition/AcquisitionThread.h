#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include "ThreadException.h"
#include "IOException.h"
#include "PacketsBuffer.h"

#include <boost/shared_ptr.hpp>

#include <pthread.h>

class AcquisitionThread {
  AcquisitionThread(const AcquisitionThread &other);
  AcquisitionThread& operator = (const AcquisitionThread &other);

  static void* threadFunction(void *arg) throw(ThreadException, IOException);
  static void threadCleanupFunction(void *arg);

  pthread_t *mThreadHandle;
  pthread_mutex_t mMutex;
  pthread_cond_t mCondition;
  pthread_attr_t mThreadAttr;
  bool bRun;
  PacketsBuffer mPacketsBuffer;

public:
  AcquisitionThread() throw(ThreadException);
  ~AcquisitionThread() throw(ThreadException);

  void run();
  void stop();
  boost::shared_ptr<VelodynePacket> getPacket() throw(IOException);

protected:

};

#endif // ACQUISITIONTHREAD_H
