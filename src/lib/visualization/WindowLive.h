#ifndef WINDOWLIVE_H
#define WINDOWLIVE_H

#include "AcquisitionThread.h"
#include "VelodyneCalibration.h"
#include "VelodynePointCloud.h"

#include <GL/gl.h>

#include <deque>

#include <stdint.h>

class WindowLive {
  WindowLive(const WindowLive &other);
  WindowLive& operator = (const WindowLive &other);

  void drawBackground(float f32Red, float f32Green, float f32Blue) const;
  void drawAxes(float f32Length) const;

  static void displayCallback();
  static void reshapeCallback(int i32Width, int i32Height);
  static void motionCallback(int i32X, int i32Y);
  static void mouseCallback(int i32Button, int i32State, int i32X, int i32Y);
  static void keyboardCallback(unsigned char u8Key, int i32X, int i32Y);
  static void timerCallback(int i32Value);

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

  AcquisitionThread mAcqThread;
  VelodyneCalibration mVelodyneCalibration;
  std::deque<GLuint> mListQueue;
  uint32_t mu32Capacity;
  uint32_t mu32Content;

public:
  WindowLive();
  WindowLive(int argc, char **argv, int i32Width = 640, int i32Height = 480);
  ~WindowLive();

  void show() const;
  void redraw() const;
  void setTranslation(double f64X, double f64Y, double f64Z);
  void setRotation(double f64Yaw, double f64Pitch, double f64Roll);
  void setScale(double f64Scale);
  void setVisibility(bool bShowAxes);
  void drawScene();

protected:

};

#endif // WINDOWLIVE_H
