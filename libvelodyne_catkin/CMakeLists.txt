cmake_minimum_required(VERSION 2.8.3)
project(libvelodyne_catkin)

find_package(catkin_simple REQUIRED)
catkin_simple()

find_package(Boost REQUIRED COMPONENTS system serialization)

add_definitions(-std=c++0x)

SET(LIBVELODYNE_SRC_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/lib)
MESSAGE(STATUS "Found libvelodyne: INCLUDE_DIRS=${LIBVELODYNE_SRC_LIB_DIR}")

include_directories(${LIBVELODYNE_SRC_LIB_DIR})

cs_add_library(${PROJECT_NAME}
  src/Velodyne32Calibration.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/base/Serializable.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/base/BinaryReader.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/base/Timestamp.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/base/BinaryBufferReader.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/base/BinaryWriter.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/exceptions/IOException.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/exceptions/SystemException.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/data-structures/VdyneScanCloud.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/data-structures/VdynePointCloud.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/com/UDPConnectionServer.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/sensor/Calibration.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/sensor/Converter.cpp
  ${LIBVELODYNE_SRC_LIB_DIR}/sensor/DataPacket.cpp
)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

cs_export(LIBRARIES ${Boost_LIBRARIES} INCLUDE_DIRS ${LIBVELODYNE_SRC_LIB_DIR})
