#include "UDPConnection.h"
#include "VelodynePacket.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <LogFile> <PktNbr>" << endl;
    return -1;
  }
  ofstream logFile (argv[1], ios::out | ios::binary);
  UDPConnection com;
  com.open();
  for (uint32_t i = 0; i < atoi(argv[2]); i++) {
    VelodynePacket vdynePacket;
    com >> vdynePacket;
    logFile << vdynePacket;
  }
  com.close();
  return 0;
}
