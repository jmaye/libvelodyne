/******************************************************************************
 * Copyright (C) 2011 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

#include "visualization/WindowLive.h"
#include "exceptions/IOException.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

#include <iostream>
#include <fstream>

using namespace std;

WindowLive::WindowLive() {
}

WindowLive::WindowLive(int argc, char **argv, int i32Width, int i32Height) :
  mi32Width(i32Width),
  mi32Height(i32Height),
  mf64Scale(1.0),
  mbShowAxes(false),
  mi32MouseButton(-1),
  mi32MouseState(-1),
  mi32StartX(0),
  mi32StartY(0),
  mf64OldRotationalInfo(0),
  mu32Content(0) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(i32Width, i32Height);
  glutInitWindowPosition(mci32InitX, mci32InitY);
  mi32ID = glutCreateWindow(argv[0]);
  glutSetWindowData(this);
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(reshapeCallback);
  glutKeyboardFunc(keyboardCallback);
  glutMouseFunc(mouseCallback);
  glutMotionFunc(motionCallback);
  mAcqThread.run();
  glutTimerFunc(mcu32TimerShot, timerCallback, 0);
  ifstream calibFile(argv[1]);
  calibFile >> mVelodyneCalibration;
}

WindowLive::WindowLive(const WindowLive &other) {
}

WindowLive& WindowLive::operator = (const WindowLive &other) {
  return *this;
}

WindowLive::~WindowLive() {
  mAcqThread.stop();
  for (uint32_t i = 0; i < mListQueue.size(); i++)
    glDeleteLists(mListQueue[i], 1);
}

void WindowLive::show() const {
  glutSetWindow(mi32ID);
  glutMainLoop();
}

void WindowLive::redraw() const {
  glutSetWindow(mi32ID);
  glutPostRedisplay();
}

void WindowLive::setTranslation(double f64X, double f64Y, double f64Z) {
  mTranslation.mX = f64X;
  mTranslation.mY = f64Y;
  mTranslation.mZ = f64Z;
  glutPostRedisplay();
}

void WindowLive::setRotation(double f64Yaw, double f64Pitch, double f64Roll) {
  mRotation.mX = f64Pitch;
  mRotation.mY = f64Yaw;
  mRotation.mZ = f64Roll;
  glutPostRedisplay();
}

void WindowLive::setScale(double f64Scale) {
  mf64Scale = f64Scale;
  glutPostRedisplay();
}

void WindowLive::setVisibility(bool bShowAxes) {
  mbShowAxes = bShowAxes;
  glutPostRedisplay();
}

void WindowLive::drawScene() {
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glLoadIdentity();
  glTranslatef(mTranslation.mX, mTranslation.mY, mTranslation.mZ);
  glRotatef(mRotation.mY, 0, 1, 0);
  glRotatef(mRotation.mX, 1, 0, 0);
  glRotatef(mRotation.mZ, 0, 0, 1);
  glScalef(mf64Scale, mf64Scale, mf64Scale);
  drawBackground(1.0f, 1.0f, 1.0f);
  if (mbShowAxes)
    drawAxes(3.0);
  try {
    VelodynePointCloud pointCloud(*(mAcqThread.getPacket()),
      mVelodyneCalibration);
    vector<Point3D>::const_iterator itStart = pointCloud.getStartIterator();
    vector<Point3D>::const_iterator itEnd = pointCloud.getEndIterator();
    vector<Point3D>::const_iterator it;
    if (pointCloud.getEndRotationAngle() < mf64OldRotationalInfo) {
      for (uint32_t i = 0; i < mListQueue.size(); i++) {
        GLuint index = mListQueue.front();
        glDeleteLists(index, 1);
        mListQueue.pop_front();
        mu32Content--;
      }
    }
    mf64OldRotationalInfo = pointCloud.getEndRotationAngle();
    GLuint index = glGenLists(1);
    glNewList(index, GL_COMPILE);
    glBegin(GL_POINTS);
    for (it = itStart; it != itEnd; it++) {
      glColor3f(0, 0, (*it).mIntensity);
      glVertex3f((*it).mX, (*it).mY, (*it).mZ);
    }
    glEnd();
    glEndList();
    mListQueue.push_back(index);
    mu32Content++;
    if (mu32Content > mcu32Capacity) {
      GLuint index = mListQueue.front();
      glDeleteLists(index, 1);
      mListQueue.pop_front();
      mu32Content--;
    }
  }
  catch (IOException &e) {
  }
  for (uint32_t i = 0; i < mListQueue.size(); i++)
    glCallList(mListQueue[i]);
  glutSwapBuffers();
  glFlush();
}

void WindowLive::drawBackground(float f32Red, float f32Green, float f32Blue)
  const {
  glClearColor(f32Red, f32Green, f32Blue, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowLive::drawAxes(float f32Length) const {
  glBegin(GL_LINES);
  glColor3f(1, 0, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(f32Length, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(0, 0, 0);
  glVertex3f(0, f32Length, 0);
  glColor3f(0, 0, 1);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, f32Length);
  glEnd();
}

void WindowLive::displayCallback() {
  WindowLive *window = (WindowLive*)glutGetWindowData();
  window->drawScene();
}

void WindowLive::reshapeCallback(int i32Width, int i32Height) {
  WindowLive *window = (WindowLive*)glutGetWindowData();
  if (i32Height == 0)
    i32Height = 1;
  window->mi32Width = i32Width;
  window->mi32Height = i32Height;
  glViewport(0, 0, window->mi32Width, window->mi32Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)window->mi32Width /
    (GLfloat)window->mi32Height, 0.1f, 200.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void WindowLive::motionCallback(int i32X, int i32Y) {
  WindowLive *window = (WindowLive*)glutGetWindowData();
  if (window->mi32MouseState == GLUT_DOWN) {
    if (window->mi32MouseButton == GLUT_LEFT_BUTTON) {
      window->mRotation.mY += (i32X - window->mi32StartX);
      window->mRotation.mX += (i32Y - window->mi32StartY);
      window->mi32StartX = i32X;
      window->mi32StartY = i32Y;
      window->redraw();
    }
    else if (window->mi32MouseButton == GLUT_RIGHT_BUTTON) {
      window->mTranslation.mX += (i32X - window->mi32StartX) /
        window->mf64Scale;
      window->mTranslation.mY -= (i32Y - window->mi32StartY) /
        window->mf64Scale;
      window->mi32StartX = i32X;
      window->mi32StartY = i32Y;
      window->redraw();
    }
  }
}

void WindowLive::mouseCallback(int i32Button, int i32State, int i32X,
  int i32Y) {
  WindowLive *window = (WindowLive*)glutGetWindowData();
  float f32DScale = 1.1;
  if (i32Button == mci32GlutWheelUp) {
    window->mf64Scale *= f32DScale;
  }
  else if (i32Button == mci32GlutWheelDown) {
    window->mf64Scale /= f32DScale;
  }
  else if (i32State == GLUT_DOWN) {
    window->mi32StartX = i32X;
    window->mi32StartY = i32Y;
  }
  window->mi32MouseButton = i32Button;
  window->mi32MouseState = i32State;
  window->redraw();
}

void WindowLive::keyboardCallback(unsigned char u8Key, int i32X, int i32Y) {
  WindowLive *window = (WindowLive*)glutGetWindowData();
  if (u8Key == 'q') {
    glutDestroyWindow(window->mi32ID);
    exit(0);
  }
}

void WindowLive::timerCallback(int i32Value) {
  WindowLive *window = (WindowLive*)glutGetWindowData();
  glutTimerFunc(mcu32TimerShot, timerCallback, 0);
  window->redraw();
}
