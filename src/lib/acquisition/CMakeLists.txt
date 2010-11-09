remake_include(../exceptions ../com ../sensor ../buffer)
remake_add_library(acquisition LINK exceptions com sensor buffer pthread)
remake_add_headers()
