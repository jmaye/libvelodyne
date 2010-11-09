remake_include(../exceptions ../com ../sensor)
remake_add_library(acquisition LINK exceptions com sensor pthread)
remake_add_headers()
