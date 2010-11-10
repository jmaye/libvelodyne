#include "VelodyneCalibration.h"
#include "VelodynePacket.h"
#include "VelodynePointCloud.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " <logFile> <calibrationFile> <asciiFile>"
         << endl;
    return -1;
  }
  ifstream logFile(argv[1], ios::in | ios::binary);
  VelodyneCalibration vdyneCalibration;
  ifstream calibFile(argv[2]);
  calibFile >> vdyneCalibration;
  ofstream asciiFile(argv[3]);
  while (logFile.eof() != true) {
    VelodynePacket vdynePacket;
    logFile >> vdynePacket;
    VelodynePointCloud pointCloud(vdynePacket, vdyneCalibration);
    asciiFile << pointCloud;
  }
  return 0;
}
