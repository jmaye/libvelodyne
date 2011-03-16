#include "AcquisitionThread.h"
#include "IOException.h"

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
  for (uint32_t i = 0; i < (uint32_t)atoi(argv[2]); i++) {
    try {
      acqThread.getPacket()->write(logFile);
    }
    catch(IOException &e) {
     i--;
    }
  }
  acqThread.stop();
  return 0;
}
