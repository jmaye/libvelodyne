#include "VelodynePacket.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <LogFilename>" << endl;
    return -1;
  }
  VelodynePacket vdynePacket;
  ifstream logFile(argv[1], ios::in | ios::binary);
  logFile >> vdynePacket;
  cout << vdynePacket;
  return 0;
}
