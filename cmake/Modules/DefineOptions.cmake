option(WITH_TESTING "Build with unit tests" OFF)
option(WITH_STATIC_LIB "Build with a static library" OFF)
if (WITH_TESTING)
  set(WITH_STATIC_LIB ON)
endif (WITH_TESTING)