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

#include "visualization/VelodyneControl.h"

#include "visualization/Velodyne.gl"

#include "ui_VelodyneControl.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

VelodyneControl::VelodyneControl(bool showVelodyne) :
    mUi(new Ui_VelodyneControl()),
    mVelodyneDisplayList(-1) {
  mUi->setupUi(this);
  connect(&View3d::getInstance().getScene(), SIGNAL(render(View3d&, Scene3d&)),
    this, SLOT(render(View3d&, Scene3d&)));
  connect(&View3d::getInstance(), SIGNAL(createDisplayLists()),
    this, SLOT(createDisplayLists()));
  setShowVelodyne(showVelodyne);
}

VelodyneControl::~VelodyneControl() {
  if (mVelodyneDisplayList != -1)
    glDeleteLists(mVelodyneDisplayList, 1);
  delete mUi;
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

void VelodyneControl::setShowVelodyne(bool showVelodyne) {
  mUi->showVelodyneCheckBox->setChecked(showVelodyne);
  View3d::getInstance().update();
}

/******************************************************************************/
/* Methods                                                                    */
/******************************************************************************/

void VelodyneControl::renderVelodyne() {
  glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_CURRENT_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);
  glCallList(mVelodyneDisplayList);
  glPopAttrib();
}

void VelodyneControl::showVelodyneToggled(bool checked) {
  setShowVelodyne(checked);
}

void VelodyneControl::render(View3d& view, Scene3d& scene) {
  if (mUi->showVelodyneCheckBox->isChecked())
    renderVelodyne();
}

void VelodyneControl::createDisplayLists() {
  mVelodyneDisplayList = genVelodyneDisplayList();
}
