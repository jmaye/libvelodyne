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

/** \file SafeQueue.h
    \brief This file defines the SafeQueue class, which represents a
           thread-safe queue.
  */

#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <deque>
#include <limits>

#include "base/Mutex.h"

/** The class SafeQueue represents a thread-safe queue.
    \brief Thread-safe queue
  */
template <typename T> class SafeQueue {
public:
  /** \name Types definitions
    @{
    */
  /// Container type
  typedef std::deque<T> Container;
  /** @}
    */

  /** \name Constructors/destructor
    @{
    */
  /// Constructs queue with capacity
  SafeQueue(size_t capacity = std::numeric_limits<size_t>::infinity);
  /// Copy constructor
  SafeQueue(const SafeQueue& other);
  /// Assignment operator
  SafeQueue& operator = (const SafeQueue& other);
  /// Destructor
  ~SafeQueue();
  /** @}
    */

  /** \name Accessors
      @{
    */
  /// Returns the capacity of the queue
  size_t getCapacity() const;
  /// Sets the capacity of the queue
  void setCapacity(size_t capacity);
  /// Returns the statistics about dropped elements
  size_t getNumDroppedElements() const;
  /// Check whether the queue is empty
  bool isEmpty() const;
  /// Get size of the queue
  size_t getSize() const;
  /** @}
    */

  /** \name Methods
      @{
    */
  /// Enqueue an element
  void enqueue(const T& value);
  /// Dequeue an element
  T dequeue();
  /** @}
    */

protected:
  /** \name Protected members
    @{
    */
  /// Queue
  Container mQueue;
  /// Capacity
  size_t mCapacity;
  /// Statistics about the dropped elements
  size_t mNumDroppedElements;
  /// Mutex protecting the queue
  mutable Mutex mMutex;
  /** @}
    */

};

#include "data-structures/SafeQueue.tpp"

#endif // SAFEQUEUE_H
