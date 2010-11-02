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

  static const double mcf64MinDistance = 90.0;
  static const double mcf64MaxDistance = 12000.0;

  struct Point3D {
    double mf64x;
    double mf64y;
    double mf64z;
  };

  std::vector<Point3D> mPointCloud;

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  virtual void read(std::ifstream &stream);
  virtual void write(std::ofstream &stream) const;

  double deg2rad(double f64Deg) const;

public:
  VelodynePointCloud();
  VelodynePointCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration);
  ~VelodynePointCloud();

protected:

};

#endif // VELODYNEPOINTCLOUD_H
