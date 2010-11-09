#include "VelodyneCalibration.h"
#include "VelodynePacket.h"
#include "VelodynePointCloud.h"
#include "WindowStatic.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <logFile> <calibrationFile>"
         << endl;
    return -1;
  }
  WindowStatic window(argc, argv);
  ifstream logFile(argv[1], ios::in | ios::binary);
  VelodyneCalibration vdyneCalibration;
  ifstream calibFile(argv[2]);
  calibFile >> vdyneCalibration;
  while (logFile.eof() != true) {
    VelodynePacket vdynePacket;
    logFile >> vdynePacket;
    VelodynePointCloud pointCloud(vdynePacket, vdyneCalibration);
    window.addPointCloud(pointCloud);
  }
  window.createGlList();
  window.setTranslation(0, -10, -60);
  window.setRotation(0, 30, 0);
  window.setVisibility(true);
  window.show();
  return 0;
}
