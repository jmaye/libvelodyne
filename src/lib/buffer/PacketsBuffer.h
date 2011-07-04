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

/** \file PacketsBuffer.h
    \brief This file defines the PacketsBuffer class, which represents buffer
           for acquisition
  */

#ifndef PACKETSBUFFER_H
#define PACKETSBUFFER_H

#include "sensor/VelodynePacket.h"
#include "exceptions/ThreadException.h"
#include "exceptions/IOException.h"

#include <boost/shared_ptr.hpp>

#include <deque>

#include <pthread.h>

/** The class PacketsBuffer represents buffer for acquisition.
    \brief Packets buffer
  */
class PacketsBuffer {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  PacketsBuffer(const PacketsBuffer& other);
  /// Assignment operator
  PacketsBuffer& operator = (const PacketsBuffer& other);
  /** @}
    */

  /** \name Private members
    @{
    */
  /// Physical buffer
  std::deque<boost::shared_ptr<VelodynePacket> > mBuffer;
  /// Mutex for accessing the buffer
  pthread_mutex_t mMutex;
  /// Capacity of the buffer
  size_t mCapacity;
  /// Actual size of the buffer
  size_t mContent;
  /// Statistics about the dropped packages
  size_t mDroppedPackages;
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Constructs buffer with given capacity
  PacketsBuffer(size_t capacity = 1000) throw (ThreadException);
  /// Destructor
  ~PacketsBuffer() throw (ThreadException);
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Gets the capacity of the buffer
  size_t getCapacity();
  /// Gets the actual size of the buffer
  size_t getContent();
  /// Returns the statistics about dropped packages
  size_t getDroppedPackages();
  /// Sets the capacity of the buffer
  void setCapacity(size_t capacity);
  /** @}
    */

  /** \name Methods
    @{
    */
  /// Adds a packet into the buffer
  void pushPacket(boost::shared_ptr<VelodynePacket> packet);
  /// Pops a packets from the buffer
  boost::shared_ptr<VelodynePacket> popPacket() throw (IOException);
  /** @}
    */

protected:

};

#endif // PACKETSBUFFER_H
