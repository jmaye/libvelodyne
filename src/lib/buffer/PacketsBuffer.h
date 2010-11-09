#ifndef PACKETSBUFFER_H
#define PACKETSBUFFER_H

#include "VelodynePacket.h"
#include "ThreadException.h"

#include <boost/shared_ptr.hpp>

#include <iosfwd>
#include <deque>

#include <pthread.h>
#include <stdint.h>

class PacketsBuffer {
  friend std::ostream& operator << (std::ostream &stream,
    const PacketsBuffer &obj);
  friend std::istream& operator >> (std::istream &stream,
    PacketsBuffer &obj);
  friend std::ofstream& operator << (std::ofstream &stream,
    const PacketsBuffer &obj);
  friend std::ifstream& operator >> (std::ifstream &stream,
    PacketsBuffer &obj);

  PacketsBuffer(const PacketsBuffer &other);
  PacketsBuffer& operator = (const PacketsBuffer &other);

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  virtual void read(std::ifstream &stream);
  virtual void write(std::ofstream &stream) const;

  std::deque<boost::shared_ptr<VelodynePacket> > mBuffer;
  pthread_mutex_t mMutex;
  uint32_t mu32Capacity;
  uint32_t mu32Content;

public:
  PacketsBuffer(uint32_t u32Capacity = 1000) throw(ThreadException);
  ~PacketsBuffer();

  void pushPacket(boost::shared_ptr<VelodynePacket> packet);
  boost::shared_ptr<VelodynePacket> popPacket();

protected:

};

#endif // PACKETSBUFFER_H
