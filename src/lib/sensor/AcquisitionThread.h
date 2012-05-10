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

/** \file AcquisitionThread.h
    \brief This file defines the AcquisitionThread class which is an interface
           for acquiring Velodyne packets using a thread.
  */

#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include <boost/shared_ptr.hpp>

#include "base/Thread.h"
#include "data-structures/SafeQueue.h"

class UDPConnectionServer;

/** The class AcquisitionThread represents an interface for acquiring Velodyne
    packets using a thread.
    \brief Velodyne acquisition thread
  */
template <typename P> class AcquisitionThread :
  public Thread {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  AcquisitionThread(const AcquisitionThread& other);
  /// Assignment operator
  AcquisitionThread& operator = (const AcquisitionThread& other);
  /** @}
    */

public:
  /** \name Types definitions
    @{
    */
  typedef SafeQueue<boost::shared_ptr<P> > Buffer;
  /** @}
    */

  /** \name Constructors/Destructor
    @{
    */
  /// Constructs thread with UDP connection and buffer size
  AcquisitionThread(UDPConnectionServer& connection, size_t bufferSize =
    std::numeric_limits<size_t>::infinity());
   /// Destructor
  ~AcquisitionThread();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Access the thread's acquisition queue
  const Buffer& getBuffer() const;
  /// Access the thread's acquisition queue
  Buffer& getBuffer();
  /** @}
    */

protected:
  /** \name Protected methods
    @{
    */
  /// Do computational processing
  virtual void process();
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// UDP connection to Velodyne
  UDPConnectionServer& mConnection;
  /// Buffer for acquisition
  Buffer mBuffer;
  /** @}
    */

};

#include "sensor/AcquisitionThread.tpp"

#endif // ACQUISITIONTHREAD_H
