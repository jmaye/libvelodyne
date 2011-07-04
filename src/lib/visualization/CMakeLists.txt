remake_add_library(visualization LINK sensor ${OPENGL_LIBRARIES}
  ${GLU_LIBRARIES} ${GLUT_LIBRARY})
remake_add_headers(INSTALL visualization)
