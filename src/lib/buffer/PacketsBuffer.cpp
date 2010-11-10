#include "PacketsBuffer.h"

using namespace boost;
using namespace std;

PacketsBuffer::PacketsBuffer(uint32_t u32Capacity) throw(ThreadException)
  : mu32Capacity(u32Capacity),
    mu32Content(0),
    mu32DroppedPackages(0) {
  if (pthread_mutex_init(&mMutex, NULL)) {
    throw ThreadException("PacketsBuffer::PacketsBuffer: mutex init failed");
  }
}

PacketsBuffer::PacketsBuffer(const PacketsBuffer &other) {
}

PacketsBuffer& PacketsBuffer::operator = (const PacketsBuffer &other) {
  return *this;
}

PacketsBuffer::~PacketsBuffer() {
  if (pthread_mutex_destroy(&mMutex)) {
    throw ThreadException("PacketsBuffer::~PacketsBuffer: mutex destroy failed");
  }
}

void PacketsBuffer::pushPacket(shared_ptr<VelodynePacket> packet) {
  pthread_mutex_lock(&mMutex);
  mBuffer.push_back(packet);
  mu32Content++;
  if (mu32Content > mu32Capacity) {
    mBuffer.pop_front();
    mu32Content--;
    mu32DroppedPackages++;
  }
  pthread_mutex_unlock(&mMutex);
}

shared_ptr<VelodynePacket> PacketsBuffer::popPacket() throw(IOException) {
  shared_ptr<VelodynePacket> packet;
  pthread_mutex_lock(&mMutex);
  if (mu32Content == 0) {
    pthread_mutex_unlock(&mMutex);
    throw IOException("PacketsBuffer::popPacket: empty queue");
  }
  packet = mBuffer.front();
  mBuffer.pop_front();
  mu32Content--;
  pthread_mutex_unlock(&mMutex);
  return packet;
}

uint32_t PacketsBuffer::getCapacity() {
  uint32_t u32Capacity;
  pthread_mutex_lock(&mMutex);
  u32Capacity = mu32Capacity;
  pthread_mutex_unlock(&mMutex);
  return u32Capacity;
}

void PacketsBuffer::setCapacity(uint32_t u32Capacity) {
  pthread_mutex_lock(&mMutex);
  mu32Capacity = u32Capacity;
  pthread_mutex_unlock(&mMutex);
}

uint32_t PacketsBuffer::getContent() {
  uint32_t u32Content;
  pthread_mutex_lock(&mMutex);
  u32Content = mu32Content;
  pthread_mutex_unlock(&mMutex);
  return u32Content;
}

uint32_t PacketsBuffer::getDroppedPackages() {
  uint32_t u32DroppedPackages;
  pthread_mutex_lock(&mMutex);
  u32DroppedPackages = mu32DroppedPackages;
  pthread_mutex_unlock(&mMutex);
  return u32DroppedPackages;
}
