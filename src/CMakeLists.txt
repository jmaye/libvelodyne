remake_find_package(Boost)
remake_find_package(OpenGL)
remake_find_package(GLU CONFIG glu)
remake_find_library(glut GL/glut.h)
remake_find_package(Threads)
if(NOT ${CMAKE_USE_PTHREADS_INIT})
  message(FATAL_ERROR "Missing POSIX thread support!")
endif(NOT ${CMAKE_USE_PTHREADS_INIT})

remake_project_header(config.h.remake)

remake_include(${Boost_INCLUDE_DIR} ${GLUT_HEADERS} ${OPENGL_INCLUDE_DIR})

remake_add_directories(lib)
remake_add_directories(bin COMPONENT test)
