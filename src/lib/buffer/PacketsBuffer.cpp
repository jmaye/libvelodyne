#include "PacketsBuffer.h"

#include <iostream>
#include <fstream>

using namespace boost;
using namespace std;

PacketsBuffer::PacketsBuffer(uint32_t u32Capacity) throw(ThreadException)
  : mu32Capacity(u32Capacity),
    mu32Content(0) {
  if (pthread_mutex_init(&mMutex, NULL)) {
    throw ThreadException("PacketsBuffer::PacketsBuffer: mutex init failed");
  }
}

PacketsBuffer::PacketsBuffer(const PacketsBuffer &other) {
}

PacketsBuffer& PacketsBuffer::operator = (const PacketsBuffer &other) {
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
  }
  pthread_mutex_unlock(&mMutex);
}

shared_ptr<VelodynePacket> PacketsBuffer::popPacket() {
  shared_ptr<VelodynePacket> packet;
  pthread_mutex_lock(&mMutex);
  packet = mBuffer.front();
  mBuffer.pop_front();
  mu32Content--;
  pthread_mutex_unlock(&mMutex);
}

void PacketsBuffer::read(istream &stream) {
}

void PacketsBuffer::write(ostream &stream) const {
}

void PacketsBuffer::read(ifstream &stream) {
}

void PacketsBuffer::write(ofstream &stream) const {
}

ostream& operator << (ostream &stream,
  const PacketsBuffer &obj) {
  obj.write(stream);
  return stream;
}

istream& operator >> (istream &stream,
  PacketsBuffer &obj) {
  obj.read(stream);
  return stream;
}

ofstream& operator << (ofstream &stream,
  const PacketsBuffer &obj) {
  obj.write(stream);
  return stream;
}

ifstream& operator >> (ifstream &stream,
  PacketsBuffer &obj) {
  obj.read(stream);
  return stream;
}
