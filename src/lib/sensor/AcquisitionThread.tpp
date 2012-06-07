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

#include "exceptions/IOException.h"
#include "com/UDPConnectionServer.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

template <typename P>
AcquisitionThread<P>::AcquisitionThread(UDPConnectionServer& connection, size_t
    bufferSize) :
    mConnection(connection),
    mBuffer(bufferSize) {
}

template <typename P>
AcquisitionThread<P>::~AcquisitionThread() {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

template <typename P>
const typename AcquisitionThread<P>::Buffer& AcquisitionThread<P>::getBuffer()
    const {
  return mBuffer;
}

template <typename P>
typename AcquisitionThread<P>::Buffer& AcquisitionThread<P>::getBuffer() {
  return mBuffer;
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

template <typename P>
void AcquisitionThread<P>::process() {
  std::shared_ptr<P> p(new P());
  try {
    p->readBinary(mConnection);
    mBuffer.enqueue(p);
  }
  catch (IOException& e) {
    std::cerr << e.what() << std::endl;
  }
}
