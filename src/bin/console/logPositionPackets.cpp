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

/** \file logDataPackets.cpp
    \brief This file is a testing binary for logging Velodyne position packets.
  */

#include <cstdlib>

#include <iostream>
#include <fstream>

#include "com/UDPConnectionServer.h"
#include "sensor/PositionPacket.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <LogFile> <PktNbr>" << std::endl;
    return -1;
  }
  std::ofstream logFile(argv[1], std::ios::out | std::ios::binary);
  UDPConnectionServer com(8308);
  com.open();
  for (size_t i = 0; i < (size_t)atoi(argv[2]); ++i) {
    PositionPacket posPacket;
    posPacket.readBinary(com);
    posPacket.writeBinary(logFile);
  }
  com.close();
  return 0;
}
