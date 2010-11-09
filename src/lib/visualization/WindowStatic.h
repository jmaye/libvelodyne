#ifndef WINDOWSTATIC_H
#define WINDOWSTATIC_H

#include "VelodynePointCloud.h"
#include "AcquisitionThread.h"
#include "VelodyneCalibration.h"

#include <vector>

#include <stdint.h>

class WindowStatic {
  WindowStatic(const WindowStatic &other);
  WindowStatic& operator = (const WindowStatic &other);

  void drawBackground(float f32Red, float f32Green, float f32Blue) const;
  void drawAxes(float f32Length) const;

  static void displayCallback();
  static void reshapeCallback(int i32Width, int i32Height);
  static void motionCallback(int i32X, int i32Y);
  static void mouseCallback(int i32Button, int i32State, int i32X, int i32Y);
  static void keyboardCallback(unsigned char u8Key, int i32X, int i32Y);

  static const int32_t mci32InitX = 100;
  static const int32_t mci32InitY = 100;
  static const int32_t mci32GlutWheelUp = 3;
  static const int32_t mci32GlutWheelDown = 4;

  int mi32ID;
  int mi32Width;
  int mi32Height;

  Point3D mTranslation;
  Point3D mRotation;

  double mf64Scale;

  bool mbShowAxes;

  int mi32MouseButton;
  int mi32MouseState;
  int mi32StartX;
  int mi32StartY;

  std::vector<Point3D> mPointCloudVector;

public:
  WindowStatic();
  WindowStatic(int argc, char **argv, int i32Width = 640, int i32Height = 480);
  ~WindowStatic();

  void show() const;
  void redraw() const;
  void setTranslation(double f64X, double f64Y, double f64Z);
  void setRotation(double f64Yaw, double f64Pitch, double f64Roll);
  void setScale(double f64Scale);
  void setVisibility(bool bShowAxes);
  void drawScene() const;
  void addPointCloud(const VelodynePointCloud &pointCloud);

protected:

};

#endif // WINDOWSTATIC_H
