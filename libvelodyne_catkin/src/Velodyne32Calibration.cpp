#include <ethz/velodyne/Velodyne32Calibration.h>

namespace ethz {
namespace velodyne {

std::shared_ptr<Velodyne32Calibration> loadVelodyne32Calibration(std::istream & calibrationInpu){
  return std::make_shared<Velodyne32Calibration>(calibrationInpu);
}

std::shared_ptr<Velodyne32Calibration> loadVelodyne32Calibration(const std::string & calibrationFilePath){
  return std::make_shared<Velodyne32Calibration>(calibrationFilePath);
}


Velodyne32Calibration::Velodyne32Calibration(const std::string & calibrationFilePath) : Calibration(32) {
  std::ifstream ifstream(calibrationFilePath);
  ifstream >> *this;
  ifstream.close();
}

Velodyne32Calibration::Velodyne32Calibration(std::istream & calibrationInput) : Calibration(32) {
  calibrationInput >> *this;
}


} /* namespace calibration */
} /* namespace aslam */
