#ifndef VELODYNEPOINTCLOUD_H
#define VELODYNEPOINTCLOUD_H

#include "VelodynePacket.h"
#include "VelodyneCalibration.h"

#include <vector>
#include <iosfwd>

struct Point3D {
  double mf64X;
  double mf64Y;
  double mf64Z;
  uint8_t mu8Intensity;
  Point3D() : mf64X(0),
              mf64Y(0),
              mf64Z(0),
              mu8Intensity(0) {
  }
  Point3D(const Point3D &other) : mf64X(other.mf64X),
                                  mf64Y(other.mf64Y),
                                  mf64Z(other.mf64Z),
                                  mu8Intensity(other.mu8Intensity) {
  }
  Point3D& operator = (const Point3D &other) {
    if (this != &other) {
      mf64X = other.mf64X;
      mf64Y = other.mf64Y;
      mf64Z = other.mf64Z;
      mu8Intensity = other.mu8Intensity;
    }
    return *this;
  }
};

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

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  virtual void read(std::ifstream &stream);
  virtual void write(std::ofstream &stream) const;

  double mf64Timestamp;
  std::vector<Point3D> mPointCloudVector;

public:
  VelodynePointCloud();
  VelodynePointCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration);
  ~VelodynePointCloud();

  double getTimestamp() const;
  std::vector<Point3D>::const_iterator getStartIterator() const;
  std::vector<Point3D>::const_iterator getEndIterator() const;
  uint32_t getPointCloudSize() const;
  void setTimestamp(double f64Timestamp);
  void pushPoint(const Point3D &point);

protected:

};

#endif // VELODYNEPOINTCLOUD_H
