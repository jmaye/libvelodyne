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

#include "exceptions/InvalidOperationException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

template <typename T>
SafeQueue<T>::SafeQueue(size_t capacity) :
    mCapacity(capacity),
    mNumDroppedElements(0) {
}

template <typename T>
SafeQueue<T>::SafeQueue(const SafeQueue& other) {
  Mutex::ScopedLock lock(mMutex);
  mQueue = other.mQueue;
  mCapacity = other.mCapacity;
  mNumDroppedElements = other.mNumDroppedElements;
}

template <typename T>
SafeQueue<T>& SafeQueue<T>::operator = (const SafeQueue& other) {
  if (this != &other) {
    Mutex::ScopedLock lock(mMutex);
    mQueue = other.mQueue;
    mCapacity = other.mCapacity;
    mNumDroppedElements = other.mNumDroppedElements;
  }
  return *this;
}

template <typename T>
SafeQueue<T>::~SafeQueue() {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

template <typename T>
size_t SafeQueue<T>::getCapacity() const {
  Mutex::ScopedLock lock(mMutex);
  return mCapacity;
}

template <typename T>
void SafeQueue<T>::setCapacity(size_t capacity) {
  Mutex::ScopedLock lock(mMutex);
  mCapacity = capacity;
}

template <typename T>
size_t SafeQueue<T>::getNumDroppedElements() const {
  Mutex::ScopedLock lock(mMutex);
  return mNumDroppedElements;
}

template <typename T>
bool SafeQueue<T>::isEmpty() const {
  Mutex::ScopedLock lock(mMutex);
  return mQueue.empty();
}

template <typename T>
size_t SafeQueue<T>::getSize() const {
  Mutex::ScopedLock lock(mMutex);
  return mQueue.size();
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

template <typename T>
void SafeQueue<T>::enqueue(const T& value) {
  Mutex::ScopedLock lock(mMutex);
  mQueue.push_back(value);
  if (mQueue.size() > mCapacity) {
    mQueue.pop_front();
    mNumDroppedElements++;
  }
}

template <typename T>
T SafeQueue<T>::dequeue() {
  Mutex::ScopedLock lock(mMutex);
  if (mQueue.empty())
    throw InvalidOperationException("SafeQueue<T>::dequeue(): empty queue");
  T front = mQueue.front();
  mQueue.pop_front();
  return front;
}
