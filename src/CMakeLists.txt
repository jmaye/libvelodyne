remake_qt4_include()
remake_find_package(Threads)
if(NOT ${CMAKE_USE_PTHREADS_INIT})
  message(FATAL_ERROR "Missing POSIX thread support!")
endif(NOT ${CMAKE_USE_PTHREADS_INIT})
remake_find_package(Freetype)
remake_find_library(ftgl FTGL/ftgl.h PACKAGE libfreetype6-dev)

remake_project_header(config.h.remake)

remake_include(${FREETYPE_INCLUDE_DIRS})

remake_add_directories(lib)
remake_add_directories(bin)
