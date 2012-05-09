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

#include "visualization/SensorBrowseControl.h"

#include <QtCore/QDateTime>
#include <QtGui/QFileDialog>

#include "sensor/Converter.h"

#include "ui_SensorBrowseControl.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

SensorBrowseControl::SensorBrowseControl(bool showPoints) :
    mUi(new Ui_SensorBrowseControl()),
    mLogFilesize(0),
    mLogFilepos(0),
    mMinDistance(Converter::mMinDistance),
    mMaxDistance(Converter::mMaxDistance) {
  mUi->setupUi(this);
  mLogMenu = mMenu.addMenu("Log");
  mLogStartAction = mLogMenu->addAction("Seek start", this,
    SLOT(logStartClicked()));
  mLogStartAction->setShortcut(Qt::Key_Home);
  mLogPlayAction = mLogMenu->addAction("Play", this,
    SLOT(logPlayTriggered()));
  mLogPlayAction->setCheckable(true);
  mLogPlayAction->setShortcut(Qt::Key_Space);
  mLogSkipAction = mLogMenu->addAction("Skip scan", this,
    SLOT(logSkipClicked()));
  mLogSkipAction->setShortcut(Qt::Key_PageDown);
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
  setLogFilename(QDir::current().path());
  setMinDistance(mMinDistance);
  setMaxDistance(mMaxDistance);
}

SensorBrowseControl::~SensorBrowseControl() {
  delete mUi;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

void SensorBrowseControl::setPointColor(const QColor& color) {
  mPalette.setColor("Point", color);
}

void SensorBrowseControl::setPointSize(double pointSize) {
  mUi->pointSizeSpinBox->setValue(pointSize);
  View3d::getInstance().update();
}

void SensorBrowseControl::setShowPoints(bool showPoints) {
  mUi->showPointsCheckBox->setChecked(showPoints);
  View3d::getInstance().update();
}

void SensorBrowseControl::setSmoothPoints(bool smoothPoints) {
  mUi->smoothPointsCheckBox->setChecked(smoothPoints);
  View3d::getInstance().update();
}

void SensorBrowseControl::setCalibrationFilename(const QString& filename) {
  mUi->calibrationEdit->setText(filename);
  const QFileInfo fileInfo(filename);
  if (fileInfo.isFile() && fileInfo.isReadable()) {
    std::ifstream calibFile(filename.toStdString());
    calibFile >> mCalibration;
  }
}

void SensorBrowseControl::setLogFilename(const QString& filename) {
  mUi->logEdit->setText(filename);
  const QFileInfo fileInfo(filename);
  if (fileInfo.isFile() && fileInfo.isReadable())
    open(filename.toAscii().constData());
  else
    close();
  mUi->logStartButton->setEnabled(mLogFile.is_open());
  mUi->logPlayButton->setEnabled(mLogFile.is_open());
  mUi->logSkipButton->setEnabled(mLogFile.is_open());
  mLogStartAction->setEnabled(mLogFile.is_open());
  mLogPlayAction->setEnabled(mLogFile.is_open());
  mLogSkipAction->setEnabled(mLogFile.is_open());
}

void SensorBrowseControl::setMinDistance(double minDistance) {
  mMinDistance = std::min(minDistance, mMaxDistance);
  mUi->minDistanceSpinBox->setValue(mMinDistance);

}

void SensorBrowseControl::setMaxDistance(double maxDistance) {
  mMaxDistance = std::max(mMinDistance, maxDistance);
  mUi->maxDistanceSpinBox->setValue(mMaxDistance);
}

/*****************************************************************************/
/* Methods                                                                   */
/*****************************************************************************/

void SensorBrowseControl::renderPoints(double size, bool smooth) {
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

void SensorBrowseControl::calibrationBrowseClicked() {
  const QString filename = QFileDialog::getOpenFileName(this,
    "Open Calibration File", mUi->calibrationEdit->text(),
    "Velodyne calibration files (*.dat)");
  if (!filename.isNull())
    setCalibrationFilename(filename);
}

void SensorBrowseControl::logBrowseClicked() {
  const QString filename = QFileDialog::getOpenFileName(this,
    "Open Log File", mUi->logEdit->text(),
    "Velodyne log files (*.*)");
  if (!filename.isNull())
    setLogFilename(filename);
}

void SensorBrowseControl::logPlayToggled() {
  mUi->logStartButton->setEnabled(!mUi->logPlayButton->isChecked());
  mUi->logSkipButton->setEnabled(!mUi->logPlayButton->isChecked());
  mUi->logEjectButton->setEnabled(!mUi->logPlayButton->isChecked());
  mUi->calibrationBrowseButton->setEnabled(!mUi->logPlayButton->isChecked());
  mUi->logBrowseButton->setEnabled(!mUi->logPlayButton->isChecked());
  mLogStartAction->setEnabled(!mUi->logPlayButton->isChecked());
  mLogSkipAction->setEnabled(!mUi->logPlayButton->isChecked());
  mLogPlayAction->setChecked(mUi->logPlayButton->isChecked());
  if (mUi->logPlayButton->isChecked())
    mTimer.start(0);
  else
    mTimer.stop();
}

void SensorBrowseControl::logPlayTriggered() {
  mUi->logPlayButton->toggle();
}

void SensorBrowseControl::logStartClicked() {
  rewind();
  mUi->logSlider->setSliderPosition(mUi->logSlider->minimum());
}

void SensorBrowseControl::logSkipClicked() {
  readPointCloud();
}

void SensorBrowseControl::timerTimeout() {
  if (readPointCloud())
    mTimer.start(0);
}

void SensorBrowseControl::colorChanged(const QString& role, const QColor&
    color) {
  View3d::getInstance().update();
}

void SensorBrowseControl::pointSizeChanged(double pointSize) {
  setPointSize(pointSize);
}

void SensorBrowseControl::showPointsToggled(bool checked) {
  setShowPoints(checked);
}

void SensorBrowseControl::smoothPointsToggled(bool checked) {
  setSmoothPoints(checked);
}

void SensorBrowseControl::minDistanceChanged(double minDistance) {
  setMinDistance(minDistance);
}

void SensorBrowseControl::maxDistanceChanged(double maxDistance) {
  setMaxDistance(maxDistance);
}

void SensorBrowseControl::render(View3d& view, Scene3d& Scene3d) {
  if (mUi->showPointsCheckBox->isChecked())
    renderPoints(mUi->pointSizeSpinBox->value(),
      mUi->smoothPointsCheckBox->isChecked());
}

double SensorBrowseControl::getProgress() const {
  if (mLogFilesize)
    return (double)mLogFilepos / mLogFilesize;
  else
    return 0.0;
}

bool SensorBrowseControl::open(const char* filename) {
  close();
  mLogFile.open(filename, std::ios::in | std::ios::binary);
  if (mLogFile.is_open()) {
    mLogFile.seekg(0, std::ios_base::end);
    mLogFilesize = mLogFile.tellg();
    mLogFile.seekg(0, std::ios_base::beg);
    mLogFilepos = 0;
    return true;
  }
  else
    return false;
}

void SensorBrowseControl::close() {
  if (mLogFile.is_open())
    mLogFile.close();
  mLogFilesize = 0;
  mLogFilepos = 0;
}

bool SensorBrowseControl::rewind() {
  if (mLogFile.is_open()) {
    mLogFile.clear();
    mLogFile.seekg(0, std::ios::beg);
    return true;
  }
  else
    return false;
}

bool SensorBrowseControl::readPacket(DataPacket& packet) {
  if (mLogFile.is_open() && !mLogFile.eof()) {
    packet.readBinary(mLogFile);
    mLogFilepos = mLogFile.tellg();
    return true;
  }
  else {
    mLogFilepos = mLogFilesize;
    return false;
  }
}

bool SensorBrowseControl::readPointCloud() {
  if (mLogFile.is_open() && !mLogFile.eof()) {
    mPointCloud.clear();
    DataPacket packet;
    size_t numPackets = 0;
    double startAngle;
    bool finished = false;
    while (1) {
      if (readPacket(packet)) {
        Converter::toPointCloud(packet, mCalibration, mPointCloud, mMinDistance,
          mMaxDistance);
        if (numPackets && (startAngle > mPointCloud.getEndRotationAngle()))
          break;
        else
          startAngle = mPointCloud.getStartRotationAngle();
        ++numPackets;
      }
      else {
        mUi->logPlayButton->toggle();
        finished = true;
        break;
      }
    }
    mUi->logSlider->setSliderPosition(mUi->logSlider->minimum() +
      (mUi->logSlider->maximum() - mUi->logSlider->minimum()) *
      getProgress());
    if (mPointCloud.getSize()) {
      const double timestamp = mPointCloud.getTimestamp();
      const QDateTime time = QDateTime::fromTime_t(timestamp);
      QString msecs;
      msecs.sprintf("%03d", (uint)((timestamp - (size_t)timestamp) * 1e3));
      mUi->timestampLabel->setText(
          time.toString("yyyy-MM-dd hh:mm:ss:" + msecs));
    }
    mUi->numPointsSpinBox->setValue(mPointCloud.getSize());
    View3d::getInstance().update();
    return !finished;
  }
  return false;
}
