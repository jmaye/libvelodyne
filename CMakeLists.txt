cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  libvelodyne
  VERSION 1.0
  RELEASE beta
  SUMMARY "A C++ based Velodyne HDL-64E S2 and HDL-32E interfacing library."
  AUTHOR "Jerome Maye"
  CONTACT "jerome.maye@mavt.ethz.ch"
  HOME "https://github.com/jmaye/libvelodyne"
  LICENSE "GNU Lesser General Public License (LGPL)"
  PREFIX velodyne-
  INSTALL /usr/
  EXTRA_CXX_FLAGS "-std=c++0x -Wall -Wextra"
)

remake_doc(
  html
  latex
  vendor
)

remake_pack_deb(
  DESCRIPTION "Main library"
)

remake_pack_deb(
  COMPONENT dev
  DESCRIPTION "Development headers"
  DEPENDS libvelodyne
)

remake_pack_deb(
  COMPONENT console-tools
  DESCRIPTION "Console tools"
  DEPENDS libvelodyne
)

remake_pack_deb(
  COMPONENT gui-tools
  DESCRIPTION "GUI tools"
  DEPENDS libvelodyne libqt4-opengl libftgl2
)

remake_pack_deb(
  COMPONENT gui-tools-dev
  DESCRIPTION "GUI tools development headers"
  DEPENDS libvelodyne-dev libvelodyne-gui-tools libqt4-opengl-dev
)

remake_pack_deb(
  COMPONENT doc
  DESCRIPTION "Documentation"
  DEPENDS libvelodyne
)

remake_distribute_deb(
  DISTRIBUTION lucid
  SECTION libs
  UPLOAD ppa:jmaye/ethz
  DEPENDS remake doxygen libqt4-opengl-dev libftgl-dev pkg-config
  PASS CMAKE_BUILD_TYPE LIBVELODYNE_GIT_REVISION
)

remake_distribute_deb(
  DISTRIBUTION precise
  SECTION libs
  UPLOAD ppa:jmaye/ethz
  DEPENDS remake doxygen libqt4-opengl-dev libftgl-dev pkg-config
  PASS CMAKE_BUILD_TYPE LIBVELODYNE_GIT_REVISION
)

remake_distribute_deb(
  DISTRIBUTION trusty
  SECTION libs
  UPLOAD ppa:jmaye/ethz
  DEPENDS remake doxygen libqt4-opengl-dev libftgl-dev pkg-config
  PASS CMAKE_BUILD_TYPE LIBVELODYNE_GIT_REVISION
)
