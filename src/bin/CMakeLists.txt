remake_find_package(GLUT)
remake_find_package(OpenGL CONFIG gl)
remake_include(../lib/com ../lib/sensor ../lib/exceptions)
remake_add_executables(LINK com sensor ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
