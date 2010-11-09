#include "WindowLive.h"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << "<calibrationFile>"
         << endl;
    return -1;
  }
  WindowLive window(argc, argv);
  window.setTranslation(0, -10, -60);
  window.setRotation(0, 30, 0);
  window.setVisibility(true);
  window.show();
  return 0;
}
