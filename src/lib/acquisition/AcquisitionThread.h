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
    \brief This file defines the AcquisitionThread class, which acquires the
           Velodyne packets
  */

#ifndef ACQUISITIONTHREAD_H
#define ACQUISITIONTHREAD_H

#include "exceptions/ThreadException.h"
#include "exceptions/IOException.h"
#include "buffer/PacketsBuffer.h"
#include "sensor/VelodynePacket.h"

#include <boost/shared_ptr.hpp>

#include <pthread.h>

/** The class AcquisitionThread is responsible for acquiring Velodyne packets.
    \brief Acquisition thread
  */
class AcquisitionThread {
  /** \name Private constructors
    @{
    */
  /// Copy constructor
  AcquisitionThread(const AcquisitionThread& other);
  /// Assignment operator
  AcquisitionThread& operator = (const AcquisitionThread& other);
  /** @}
    */

  /** \name Private methods
    @{
    */
  /// Thread function
  static void* threadFunction(void* arg) throw (ThreadException, IOException);
  /// Cleanup thread function
  static void threadCleanupFunction(void* arg);
  /** @}
    */

  /** \name Private members
    @{
    */
  /// Thread handle
  pthread_t* mThreadHandle;
  /// Mutex for the buffer access
  pthread_mutex_t mMutex;
  /// Condition for the mutex
  pthread_cond_t mCondition;
  /// Attributes for the thread
  pthread_attr_t mThreadAttr;
  /// Is the thread running?
  bool mbRun;
  /// Buffer for the acquisition
  PacketsBuffer mPacketsBuffer;
  /** @}
    */

public:
  /** \name Constructors/destructor
    @{
    */
  /// Default constructor
  AcquisitionThread() throw (ThreadException);
  /// Destructor
  ~AcquisitionThread() throw (ThreadException);
  /** @}
    */

 /** \name Methods
    @{
    */
  /// Launches the acquisition
  void run();
  /// Stops the acquisition
  void stop();
  /// Retrieves a packet from the queue
  boost::shared_ptr<VelodynePacket> getPacket() throw (IOException);
  /// Returns the queue capacity
  size_t getQueueCapacity();
  /// Returns the actual queue size
  size_t getQueueContent();
  /// Returns the statistics about dropped packages
  size_t getQueueDroppedPackages();
  /// Sets the queue capacity
  void setQueueCapacity(size_t capacity);
  /** @}
    */

protected:

};

#endif // ACQUISITIONTHREAD_H
