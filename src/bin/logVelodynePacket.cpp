#include "UDPConnection.h"
#include "VelodynePacket.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <LogFilename>" << endl;
    return -1;
  }
  UDPConnection com;
  com.open();
  VelodynePacket vdynePacket;
  com >> vdynePacket;
  cout << vdynePacket;
  ofstream logFile (argv[1], ios::out | ios::binary);
  logFile << vdynePacket;
  com.close();
  return 0;
}
