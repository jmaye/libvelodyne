#include "Connection.h"

#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

using namespace std;

static double seconds() {
  struct timeval now;
  if (gettimeofday(&now, 0L)) {
    perror("seconds");
    exit(1);
  } else
    return (double)((double)now.tv_sec + ((double)now.tv_usec / 1000000.0));
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <LogFilename>" << endl;
    return -1;
  }
  Connection com;
  com.open();
  while (1) {
    const VelodynePacket& packet = com.receivePacket();
    ofstream logFile(argv[1], ios_base::app);
    logFile << fixed << seconds() << " ";
    for (uint32_t i = 0; i < 12; i++) {
      logFile << hex << packet.mData[i].mu16HeaderInfo << " " << dec <<
        packet.mData[i].mu16RotationalInfo << " ";
      for (uint32_t j = 0; j < 32; j++) {
        logFile << packet.mData[i].maLaserData[j].mu16Distance << " "
                << (int)packet.mData[i].maLaserData[j].mu8Intensity << " ";
      }
    }
    logFile << packet.mu16SpinCount << endl;
    logFile.close();
  }
  com.close();
  return 0;
}
