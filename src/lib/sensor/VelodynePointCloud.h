#ifndef VELODYNEPOINTCLOUD_H
#define VELODYNEPOINTCLOUD_H

#include "VelodynePacket.h"
#include "VelodyneCalibration.h"

#include <vector>
#include <iosfwd>

class VelodynePointCloud {
  friend std::ostream& operator << (std::ostream &stream,
    const VelodynePointCloud &obj);
  friend std::istream& operator >> (std::istream &stream,
    VelodynePointCloud &obj);
  friend std::ofstream& operator << (std::ofstream &stream,
    const VelodynePointCloud &obj);
  friend std::ifstream& operator >> (std::ifstream &stream,
    VelodynePointCloud &obj);

  VelodynePointCloud(const VelodynePointCloud &other);
  VelodynePointCloud& operator = (const VelodynePointCloud &other);

  static const double mcf64MinDistance = 150.0;
  static const uint16_t mcu16MeterConversion = 100;

  struct Point3D {
    double mf64X;
    double mf64Y;
    double mf64Z;
  };

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  virtual void read(std::ifstream &stream);
  virtual void write(std::ofstream &stream) const;

  double mf64Timestamp;

public:
  VelodynePointCloud();
  VelodynePointCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration);
  ~VelodynePointCloud();

  std::vector<Point3D> mPointCloudVector;

protected:

};

#endif // VELODYNEPOINTCLOUD_H
