#ifndef HB7DDB357_AB3A_496A_8248_C3CA0280F246
#define HB7DDB357_AB3A_496A_8248_C3CA0280F246

#include <memory>
#include <string>
#include <iosfwd>

namespace ethz {
namespace velodyne {
class Velodyne32Calibration;

std::shared_ptr<Velodyne32Calibration> loadVelodyne32Calibration(const std::string & calibrationFilePath);
std::shared_ptr<Velodyne32Calibration> loadVelodyne32Calibration(std::istream & calibrationInput);

} /* namespace calibration */
} /* namespace aslam */

#endif /* HB7DDB357_AB3A_496A_8248_C3CA0280F246 */
