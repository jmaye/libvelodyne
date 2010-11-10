#include "VelodyneControl.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <RPM value between 300 and 900>" << endl;
    return -1;
  }
  VelodyneControl controller;
  controller.setRPM((uint32_t)atoi(argv[1]));
  return 0;
}
