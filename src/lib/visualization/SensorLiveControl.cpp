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

#include "visualization/SensorLiveControl.h"

#include <QtCore/QDateTime>
#include <QtGui/QFileDialog>

#include "sensor/Converter.h"

#include "ui_SensorLiveControl.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

SensorLiveControl::SensorLiveControl(
    AcquisitionThread<DataPacket>::Buffer& acqBuffer, bool showPoints) :
    mUi(new Ui_SensorLiveControl()),
    mMinDistance(Converter::mMinDistance),
    mMaxDistance(Converter::mMaxDistance),
    mAcqBuffer(acqBuffer) {
  mUi->setupUi(this);
  mTimer.setSingleShot(true);
  connect(&mTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
  mUi->colorChooser->setPalette(&mPalette);
  connect(&mPalette, SIGNAL(colorChanged(const QString&, const QColor&)),
    this, SLOT(colorChanged(const QString&, const QColor&)));
  connect(&View3d::getInstance().getScene(), SIGNAL(render(View3d&, Scene3d&)),
    this, SLOT(render(View3d&, Scene3d&)));
  setPointColor(Qt::gray);
  setPointSize(1.0);
  setShowPoints(showPoints);
  setSmoothPoints(true);
  setCalibrationFilename(mUi->calibrationEdit->text());
  setMinDistance(mMinDistance);
  setMaxDistance(mMaxDistance);
  mTimer.start(0);
}

SensorLiveControl::~SensorLiveControl() {
  delete mUi;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

void SensorLiveControl::setPointColor(const QColor& color) {
  mPalette.setColor("Point", color);
}

void SensorLiveControl::setPointSize(double pointSize) {
  mUi->pointSizeSpinBox->setValue(pointSize);
  View3d::getInstance().update();
}

void SensorLiveControl::setShowPoints(bool showPoints) {
  mUi->showPointsCheckBox->setChecked(showPoints);
  View3d::getInstance().update();
}

void SensorLiveControl::setSmoothPoints(bool smoothPoints) {
  mUi->smoothPointsCheckBox->setChecked(smoothPoints);
  View3d::getInstance().update();
}

void SensorLiveControl::setCalibrationFilename(const QString& filename) {
  mUi->calibrationEdit->setText(filename);
  const QFileInfo fileInfo(filename);
  if (fileInfo.isFile() && fileInfo.isReadable()) {
    std::ifstream calibFile(filename.toStdString());
    calibFile >> mCalibration;
  }
}

void SensorLiveControl::setMinDistance(double minDistance) {
  mMinDistance = std::min(minDistance, mMaxDistance);
  mUi->minDistanceSpinBox->setValue(mMinDistance);

}

void SensorLiveControl::setMaxDistance(double maxDistance) {
  mMaxDistance = std::max(mMinDistance, maxDistance);
  mUi->maxDistanceSpinBox->setValue(mMaxDistance);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void SensorLiveControl::renderPoints(double size, bool smooth) {
  glPushAttrib(GL_CURRENT_BIT);
  if (size > 1.0)
    glPointSize(size);
  else
    glPointSize(1.0);
  if (smooth)
    glEnable(GL_POINT_SMOOTH);
  else
    glDisable(GL_POINT_SMOOTH);
  glBegin(GL_POINTS);
  View3d::getInstance().setColor(mPalette, "Point");
  for (auto it = mPointCloud.getPointBegin(); it != mPointCloud.getPointEnd();
      ++it)
    glVertex3f(it->mX, it->mY, it->mZ);
  glEnd();
  glPointSize(1.0);
  glDisable(GL_POINT_SMOOTH);
  glPopAttrib();
}

void SensorLiveControl::calibrationBrowseClicked() {
  const QString filename = QFileDialog::getOpenFileName(this,
    "Open Calibration File", mUi->calibrationEdit->text(),
    "Velodyne calibration files (*.dat)");
  if (!filename.isNull())
    setCalibrationFilename(filename);
}

void SensorLiveControl::timerTimeout() {
  if (readPointCloud()) {
    mPointCloud = mAcqPointCloud;
    View3d::getInstance().update();
    mTimer.start(0);
    emit start();
  }
  else {
    mTimer.start(1);
    emit stop();
  }
}

void SensorLiveControl::colorChanged(const QString& role, const QColor&
    color) {
  View3d::getInstance().update();
}

void SensorLiveControl::pointSizeChanged(double pointSize) {
  setPointSize(pointSize);
}

void SensorLiveControl::showPointsToggled(bool checked) {
  setShowPoints(checked);
}

void SensorLiveControl::smoothPointsToggled(bool checked) {
  setSmoothPoints(checked);
}

void SensorLiveControl::minDistanceChanged(double minDistance) {
  setMinDistance(minDistance);
}

void SensorLiveControl::maxDistanceChanged(double maxDistance) {
  setMaxDistance(maxDistance);
}

void SensorLiveControl::render(View3d& view, Scene3d& Scene3d) {
  if (mUi->showPointsCheckBox->isChecked())
    renderPoints(mUi->pointSizeSpinBox->value(),
      mUi->smoothPointsCheckBox->isChecked());
}

bool SensorLiveControl::readPacket(std::shared_ptr<DataPacket>& packet) {
  if (mAcqBuffer.isEmpty())
    return false;
  else {
    packet = mAcqBuffer.dequeue();
    return true;
  }
}

bool SensorLiveControl::readPointCloud() {
  mAcqPointCloud.clear();
  std::shared_ptr<DataPacket> packet;
  size_t numPackets = 0;
  double startAngle;
  while (1) {
    if (readPacket(packet)) {
      Converter::toPointCloud(*packet, mCalibration, mAcqPointCloud,
        mMinDistance, mMaxDistance);
      if (numPackets && (startAngle > mAcqPointCloud.getEndRotationAngle()))
        break;
      else
        startAngle = mAcqPointCloud.getStartRotationAngle();
      ++numPackets;
    }
    else
      return false;
    }
    if (mAcqPointCloud.getSize()) {
      const double timestamp = mAcqPointCloud.getTimestamp();
      const QDateTime time = QDateTime::fromTime_t(timestamp);
      QString msecs;
      msecs.sprintf("%03d", (uint)((timestamp - (size_t)timestamp) * 1e3));
      mUi->timestampLabel->setText(
          time.toString("yyyy-MM-dd hh:mm:ss:" + msecs));
    }
    mUi->numPointsSpinBox->setValue(mAcqPointCloud.getSize());
    return true;
}
