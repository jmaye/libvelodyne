#include "AcquisitionThread.h"

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <fstream>

#include <stdlib.h>

using namespace boost;
using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <LogFile> <PktNbr>" << endl;
    return -1;
  }
  ofstream logFile (argv[1], ios::out | ios::binary);
  AcquisitionThread acqThread;
  acqThread.run();
  for (uint32_t i = 0; i < atoi(argv[2]); i++) {
    shared_ptr<VelodynePacket> vdynePacket = acqThread.getPacket();
    logFile << *vdynePacket;
  }
  acqThread.stop();
  return 0;
}
