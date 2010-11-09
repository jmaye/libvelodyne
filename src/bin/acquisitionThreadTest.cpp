#include "AcquisitionThread.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  AcquisitionThread acquisition;
  while (1) {
    char input;
    cin >> input;
    if (input == 'r')
      acquisition.run();
    else if (input == 's')
      acquisition.stop();
    else if (input == 'q')
      break;
  }
  return 0;
}
