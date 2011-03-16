#include "VelodyneCalibration.h"
#include "VelodynePacket.h"
#include "VelodynePointCloud.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " <logFile> <calibrationFile> <wrmlFile>"
         << endl;
    return -1;
  }
  ifstream logFile(argv[1], ios::in | ios::binary);
  VelodyneCalibration vdyneCalibration;
  ifstream calibFile(argv[2]);
  calibFile >> vdyneCalibration;
  ofstream wrmlFile(argv[3]);
  wrmlFile << "#VRML V2.0 utf8" << endl
           << "Shape {" << endl
           << "   geometry PointSet {" << endl
           << "      coord Coordinate {" << endl
           << "         point [" << endl;
  while (logFile.eof() != true) {
    VelodynePacket vdynePacket;
    vdynePacket.read(logFile);
    VelodynePointCloud pointCloud(vdynePacket, vdyneCalibration);
    wrmlFile << pointCloud;
  }
  wrmlFile << "         ]" << endl
           << "      }" << endl
           << "   }" << endl
           << "}" << endl;
  return 0;
}
