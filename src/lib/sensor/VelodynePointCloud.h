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
public:
  typedef std::vector<Point3D> Point3DVector;
  typedef Point3DVector::const_iterator Point3DVectorConstIterator;
  
private:
  friend std::ostream& operator << (std::ostream &stream,
    const VelodynePointCloud &obj);
  friend std::istream& operator >> (std::istream &stream,
    VelodynePointCloud &obj);

  VelodynePointCloud(const VelodynePointCloud &other);
  VelodynePointCloud& operator = (const VelodynePointCloud &other);

  static const double mcf64MinDistance = 1.5;
  static const double mcf64MaxDistance = 120.0;
  static const uint16_t mcu16MeterConversion = 100;

  virtual void readFormatted(std::istream &stream);
  virtual void writeFormatted(std::ostream &stream) const;

  double mf64Timestamp;
  Point3DVector mPointCloudVector;
  double mf64StartRotationAngle;
  double mf64EndRotationAngle;

public:
  VelodynePointCloud();
  VelodynePointCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration,
    double mcf64MinDistance = VelodynePointCloud::mcf64MinDistance,
    double mcf64MaxDistance = VelodynePointCloud::mcf64MaxDistance);
  ~VelodynePointCloud();

  double getTimestamp() const;
  double getStartRotationAngle() const;
  double getEndRotationAngle() const;
  Point3DVectorConstIterator getStartIterator() const;
  Point3DVectorConstIterator getEndIterator() const;
  uint32_t getSize() const;
  void setTimestamp(double f64Timestamp);
  void setStartRotationAngle(double f64Angle);
  void setEndRotationAngle(double f64Angle);
  void pushPoint(const Point3D &point);

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  
protected:

};

#endif // VELODYNEPOINTCLOUD_H
