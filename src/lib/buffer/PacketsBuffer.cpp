/******************************************************************************
 * Copyright (C) 2011 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include "buffer/PacketsBuffer.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

PacketsBuffer::PacketsBuffer(size_t capacity) throw (ThreadException) :
  mCapacity(capacity),
  mContent(0),
  mDroppedPackages(0) {
  if (pthread_mutex_init(&mMutex, NULL)) {
    throw ThreadException("PacketsBuffer::PacketsBuffer(): mutex init failed");
  }
}

PacketsBuffer::PacketsBuffer(const PacketsBuffer& other) {
}

PacketsBuffer& PacketsBuffer::operator = (const PacketsBuffer& other) {
  return *this;
}

PacketsBuffer::~PacketsBuffer() throw (ThreadException) {
  if (pthread_mutex_destroy(&mMutex)) {
    throw ThreadException("PacketsBuffer::~PacketsBuffer(): mutex destroy failed");
  }
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

size_t PacketsBuffer::getCapacity() {
  size_t capacity;
  pthread_mutex_lock(&mMutex);
  capacity = mCapacity;
  pthread_mutex_unlock(&mMutex);
  return capacity;
}

void PacketsBuffer::setCapacity(size_t capacity) {
  pthread_mutex_lock(&mMutex);
  mCapacity = capacity;
  pthread_mutex_unlock(&mMutex);
}

size_t PacketsBuffer::getContent() {
  size_t content;
  pthread_mutex_lock(&mMutex);
  content = mContent;
  pthread_mutex_unlock(&mMutex);
  return content;
}

size_t PacketsBuffer::getDroppedPackages() {
  size_t droppedPackages;
  pthread_mutex_lock(&mMutex);
  droppedPackages = mDroppedPackages;
  pthread_mutex_unlock(&mMutex);
  return droppedPackages;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void PacketsBuffer::pushPacket(boost::shared_ptr<VelodynePacket> packet) {
  pthread_mutex_lock(&mMutex);
  mBuffer.push_back(packet);
  mContent++;
  if (mContent > mCapacity) {
    mBuffer.pop_front();
    mContent--;
    mDroppedPackages++;
  }
  pthread_mutex_unlock(&mMutex);
}

boost::shared_ptr<VelodynePacket> PacketsBuffer::popPacket()
  throw (IOException) {
  boost::shared_ptr<VelodynePacket> packet;
  pthread_mutex_lock(&mMutex);
  if (mContent == 0) {
    pthread_mutex_unlock(&mMutex);
    throw IOException("PacketsBuffer::popPacket(): empty queue");
  }
  packet = mBuffer.front();
  mBuffer.pop_front();
  mContent--;
  pthread_mutex_unlock(&mMutex);
  return packet;
}
