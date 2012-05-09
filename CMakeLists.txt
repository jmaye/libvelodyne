cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_set(CMAKE_CXX_FLAGS -std=c++0x CACHE FORCE INIT)

remake_project(
  libvelodyne
  VERSION 1.0
  RELEASE beta
  SUMMARY "A C++ based Velodyne HDL-64E S2 and HDL-32E interfacing library"
  AUTHOR "Jerome Maye"
  CONTACT "jerome.maye@mavt.ethz.ch"
  HOME "http://aslforge.ethz.ch/projects/libvelodyne"
  LICENSE "GNU Lesser General Public License (LGPL)"
  PREFIX velodyne-
  INSTALL /usr/
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
  libvelodyne
  libboost-dev
)

remake_pack_deb(
  COMPONENT console-tools
  DESCRIPTION "Console tools"
  libvelodyne
)

remake_pack_deb(
  COMPONENT gui-tools
  DESCRIPTION "GUI tools"
  libvelodyne
  libqt4-gui
  libqt4-opengl
  libfreetype6
  libftgl2
)

remake_pack_deb(
  COMPONENT doc
  DESCRIPTION "Documentation"
  libvelodyne
)
