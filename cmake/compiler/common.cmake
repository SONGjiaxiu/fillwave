set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (FILLWAVE_COMPILATION_RELEASE)
  set (CMAKE_BUILD_TYPE Release)
else()
  set (CMAKE_BUILD_TYPE Debug)
endif ()