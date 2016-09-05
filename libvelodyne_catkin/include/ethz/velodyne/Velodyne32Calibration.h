#ifndef HB7DDB357_AB3A_496A_8248_C3CA0280F247
#define HB7DDB357_AB3A_496A_8248_C3CA0280F247

#include "Velodyne32CalibrationForward.h"
#include <sensor/Calibration.h>

namespace ethz {
namespace velodyne {

class Velodyne32Calibration : public ::Calibration {
 public:
  Velodyne32Calibration(const std::string & calibrationFilePath);
  Velodyne32Calibration(std::istream & calibrationInput);
};

} /* namespace calibration */
} /* namespace aslam */

#endif /* HB7DDB357_AB3A_496A_8248_C3CA0280F247 */
