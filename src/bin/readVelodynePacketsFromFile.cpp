#include "VelodynePacket.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <LogFile>" << endl;
    return -1;
  }
  ifstream logFile(argv[1], ios::in | ios::binary);
  while (logFile.eof() != true) {
    VelodynePacket vdynePacket;
    logFile >> vdynePacket;
    cout << vdynePacket;
  }
  return 0;
}
