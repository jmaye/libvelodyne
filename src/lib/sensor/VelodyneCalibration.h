#ifndef VELODYNECALIBRATION_H
#define VELODYNECALIBRATION_H

#include "IOException.h"
#include "OutOfBoundException.h"

#include <iosfwd>

#include <stdint.h>

class VelodyneCalibration {
  friend std::ostream& operator << (std::ostream &stream,
    const VelodyneCalibration &obj);
  friend std::istream& operator >> (std::istream &stream,
    VelodyneCalibration &obj);

  VelodyneCalibration(const VelodyneCalibration &other);
  VelodyneCalibration& operator = (const VelodyneCalibration &other);

  virtual void readFormatted(std::istream &stream) throw(IOException);
  virtual void writeFormatted(std::ostream &stream) const;

  static const uint16_t mcu16LasersNbr = 64;

  struct VelodyneLaserCorrection {
    double mf64RotCorr;
    double mf64SinRotCorr;
    double mf64CosRotCorr;
    double mf64VertCorr;
    double mf64SinVertCorr;
    double mf64CosVertCorr;
    double mf64DistCorr;
    double mf64VertOffsCorr;
    double mf64HorizOffsCorr;
    VelodyneLaserCorrection() : mf64RotCorr(0),
                                mf64SinRotCorr(0),
                                mf64CosRotCorr(0),
                                mf64VertCorr(0),
                                mf64SinVertCorr(0),
                                mf64CosVertCorr(0),
                                mf64DistCorr(0),
                                mf64VertOffsCorr(0),
                                mf64HorizOffsCorr(0) {
    }
    VelodyneLaserCorrection(const VelodyneLaserCorrection &other)
      : mf64RotCorr(other.mf64RotCorr),
        mf64SinRotCorr(other.mf64SinRotCorr),
        mf64CosRotCorr(other.mf64CosRotCorr),
        mf64VertCorr(other.mf64VertCorr),
        mf64SinVertCorr(other.mf64SinVertCorr),
        mf64CosVertCorr(other.mf64CosVertCorr),
        mf64DistCorr(other.mf64DistCorr),
        mf64VertOffsCorr(other.mf64VertOffsCorr),
        mf64HorizOffsCorr(other.mf64HorizOffsCorr) {
    }
    VelodyneLaserCorrection& operator = (const VelodyneLaserCorrection &other) {
      if (this != &other) {
        mf64RotCorr = other.mf64RotCorr;
        mf64SinRotCorr = other.mf64SinRotCorr;
        mf64CosRotCorr =other.mf64CosRotCorr;
        mf64VertCorr = other.mf64VertCorr;
        mf64SinVertCorr = other.mf64SinVertCorr;
        mf64CosVertCorr = other.mf64CosVertCorr;
        mf64DistCorr =other.mf64DistCorr;
        mf64VertOffsCorr = other.mf64VertOffsCorr;
        mf64HorizOffsCorr = other.mf64HorizOffsCorr;
      }
      return *this;
    }
  };

  VelodyneLaserCorrection mCorr[mcu16LasersNbr];

public:
  VelodyneCalibration();
  ~VelodyneCalibration();

  double getRotCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getSinRotCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getCosRotCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getVertCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getSinVertCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getCosVertCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getDistCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getVertOffsCorr(uint16_t u16LaserNbr) const throw(OutOfBoundException);
  double getHorizOffsCorr(uint16_t u16LaserNbr) const
    throw(OutOfBoundException);

  void setRotCorr(uint16_t u16LaserNbr, double mf64Value)
    throw(OutOfBoundException);
  void setVertCorr(uint16_t u16LaserNbr, double mf64Value)
    throw(OutOfBoundException);
  void setDistCorr(uint16_t u16LaserNbr, double mf64Value)
    throw(OutOfBoundException);
  void setVertOffsCorr(uint16_t u16LaserNbr, double mf64Value)
    throw(OutOfBoundException);
  void setHorizOffsCorr(uint16_t u16LaserNbr, double mf64Value)
    throw(OutOfBoundException);

  double deg2rad(double f64Deg) const;
  double rad2deg(double f64Rad) const;

  virtual void read(std::istream &stream);
  virtual void write(std::ostream &stream) const;

protected:

};

#endif // VELODYNECALIBRATION_H
