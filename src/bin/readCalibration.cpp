#include "VelodyneCalibration.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <calibrationFile>" << endl;
    return -1;
  }
  VelodyneCalibration vdyneCalibration;
  ifstream calibFile(argv[1]);
  calibFile >> vdyneCalibration;
  cout << vdyneCalibration;
  return 0;
}
