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

/** \file OutOfBoundException.h
    \brief This file defines the OutOfBoundException class, which represents any
           attempts to access unallocated memory
  */

#ifndef OUTOFBOUNDEXCEPTION_H
#define OUTOFBOUNDEXCEPTION_H

#include <stdexcept>
#include <string>

/** The class OutOfBoundException represents any attempts to access unallocated
    memory regions.
    \brief Out of bounds exception
  */
class OutOfBoundException : public std::range_error {
  /** \name Private constructors
    @{
    */
  /// Assignment operator
  OutOfBoundException& operator = (const OutOfBoundException& other);
  /** @}
    */

public:
  /** \name Constructors/Destructor
    @{
    */
  /// Constructs exception from message
  OutOfBoundException(const std::string& msg = "");
  /// Copy constructor
  OutOfBoundException(const OutOfBoundException& other);
  /** @}
    */

protected:

};

#endif // OUTOFBOUNDEXCEPTION_H
