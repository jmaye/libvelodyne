#ifndef VELODYNESCANCLOUD_H
#define VELODYNESCANCLOUD_H

#include "VelodynePacket.h"
#include "VelodyneCalibration.h"

#include <vector>
#include <iosfwd>

struct Scan {
  double mf64Range;
  double mf64Heading;
  double mf64Pitch;
  Scan() : mf64Range(0),
           mf64Heading(0),
           mf64Pitch(0) {
  }
  Scan(const Scan &other) : mf64Range(other.mf64Range),
                            mf64Heading(other.mf64Heading),
                            mf64Pitch(other.mf64Pitch) {
  }
  Scan& operator = (const Scan &other) {
    if (this != &other) {
      mf64Range = other.mf64Range;
      mf64Heading = other.mf64Heading;
      mf64Pitch = other.mf64Pitch;
    }
    return *this;
  }
};

class VelodyneScanCloud {
  friend std::ostream& operator << (std::ostream &stream,
    const VelodyneScanCloud &obj);
  friend std::istream& operator >> (std::istream &stream,
    VelodyneScanCloud &obj);
  friend std::ofstream& operator << (std::ofstream &stream,
    const VelodyneScanCloud &obj);
  friend std::ifstream& operator >> (std::ifstream &stream,
    VelodyneScanCloud &obj);

  VelodyneScanCloud(const VelodyneScanCloud &other);
  VelodyneScanCloud& operator = (const VelodyneScanCloud &other);

  static const double mcf64MinDistance = 150.0;
  static const uint16_t mcu16MeterConversion = 100;

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;
  virtual void read(std::ifstream &stream);
  virtual void write(std::ofstream &stream) const;

  double normalizeAnglePositive(double f64Angle) const;

  double normalizeAngle(double f64Angle) const;

  std::vector<Scan> mScanCloudVector;
  double mf64Timestamp;

public:
  VelodyneScanCloud();
  VelodyneScanCloud(const VelodynePacket &vdynePacket,
    const VelodyneCalibration &vdyneCalibration);
  ~VelodyneScanCloud();

  double getTimestamp() const;
  std::vector<Scan>::const_iterator getStartIterator() const;
  std::vector<Scan>::const_iterator getEndIterator() const;
  void setTimestamp(double f64Timestamp);
  void pushScan(const Scan &scan);

protected:

};

#endif // VELODYNESCANCLOUD_H
