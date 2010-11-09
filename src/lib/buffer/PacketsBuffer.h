#ifndef PACKETSBUFFER_H
#define PACKETSBUFFER_H

#include "VelodynePacket.h"
#include "ThreadException.h"
#include "IOException.h"

#include <boost/shared_ptr.hpp>

#include <deque>

#include <pthread.h>
#include <stdint.h>

class PacketsBuffer {
  PacketsBuffer(const PacketsBuffer &other);
  PacketsBuffer& operator = (const PacketsBuffer &other);

  std::deque<boost::shared_ptr<VelodynePacket> > mBuffer;
  pthread_mutex_t mMutex;
  uint32_t mu32Capacity;
  uint32_t mu32Content;

public:
  PacketsBuffer(uint32_t u32Capacity = 1000) throw(ThreadException);
  ~PacketsBuffer();

  void pushPacket(boost::shared_ptr<VelodynePacket> packet);
  boost::shared_ptr<VelodynePacket> popPacket() throw(IOException);
  uint32_t getCapacity();
  uint32_t getContent();
  void setCapacity(uint32_t u32Capacity);

protected:

};

#endif // PACKETSBUFFER_H
