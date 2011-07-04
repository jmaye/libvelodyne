remake_find_package(OpenGL CONFIG gl)
remake_find_package(GLU CONFIG glu)
remake_find_library(glut GL/glut.h)

remake_add_library(visualization LINK sensor ${OPENGL_LIBRARIES}
  ${GLU_LIBRARIES} ${GLUT_LIBRARY})
remake_add_headers()
