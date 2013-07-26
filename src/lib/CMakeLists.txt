remake_include()

remake_add_directories(base)
remake_add_directories(com)
remake_add_directories(data-structures)
remake_add_directories(exceptions)
remake_add_directories(sensor)
remake_pkg_config_generate(EXTRA_CFLAGS -std=c++0x)

remake_add_directories(visualization COMPONENT gui-tools)
remake_pkg_config_generate(COMPONENT gui-tools REQUIRES libvelodyne)
