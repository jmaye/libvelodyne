remake_project_header(Config.h.remake)
remake_find_file(boost/shared_ptr.hpp PACKAGE libboost-dev)
remake_add_directories(lib)
remake_add_directories(bin COMPONENT test)
