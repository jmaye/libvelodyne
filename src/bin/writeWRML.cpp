#include "VelodyneCalibration.h"
#include "VelodynePacket.h"
#include "VelodynePointCloud.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " <logFile calibrationFile wrmlFile>"
         << endl;
    return -1;
  }
  VelodynePacket vdynePacket;
  ifstream logFile(argv[1], ios::in | ios::binary);
  logFile >> vdynePacket;
  VelodyneCalibration vdyneCalibration;
  ifstream calibFile(argv[2]);
  calibFile >> vdyneCalibration;
  VelodynePointCloud pointCloud(vdynePacket, vdyneCalibration);
  ofstream wrmlFile(argv[3]);
  wrmlFile << "#VRML V2.0 utf8" << endl
           << "Shape {" << endl
           << "   geometry PointSet {" << endl
           << "      coord Coordinate {" << endl
           << "         point [" << endl;
  wrmlFile << pointCloud;
  wrmlFile << "         ]" << endl
           << "      }" << endl
           << "   }" << endl
           << "}" << endl;

  return 0;
}
