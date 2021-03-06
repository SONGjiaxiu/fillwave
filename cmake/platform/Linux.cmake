cmake_minimum_required (VERSION 2.8.8)

cmake_policy (SET CMP0048 NEW)

# -----------------------------------------------
# Package type
# -----------------------------------------------

message ("Building binary package")
project (libfillwave VERSION 10.0.0 LANGUAGES C CXX)

# -----------------------------------------------
# Includes
# -----------------------------------------------

include_directories (${FILLWAVE_PATH_INCLUDE}
    ${FILLWAVE_EXT_INCLUDES}
    ${FILLWAVE_EXT_FONTGENERATOR_INCLUDES}
    ${FILLWAVE_TEXTURE_WRITER_INCLUDES}
    ${FILLWAVE_TEXTURE_LOADER_INCLUDES}
    /usr/include/freetype2) #uglt freetype2 needs /usr/local/include/freetype2/ft2build.h

if (FILLWAVE_BUILD_PACK)
  include_directories (${FILLWAVE_MODEL_LOADER_INCLUDES})
endif ()

if (FILLWAVE_BUILD_PACK)
  include_directories (${FILLWAVE_EXT_GLM_INCLUDES})
endif ()

# -----------------------------------------------
# Targets
# -----------------------------------------------

add_library (fillwave SHARED ${FILLWAVE_SOURCES})

# -----------------------------------------------
# Subprojects
# -----------------------------------------------

add_subdirectory (ext)

if (FILLWAVE_BUILD_PACK)
  add_subdirectory (${FILLWAVE_MODEL_LOADER_PATH})
  add_subdirectory (${FILLWAVE_EXT_FREETYPE2_PATH})
  add_subdirectory (${FILLWAVE_EXT_GLFW_PATH}) # needs randr libraries
endif ()

# -----------------------------------------------
# Linker
# -----------------------------------------------

include_directories (${FILLWAVE_BACKEND_INCLUDES})

add_dependencies (fillwave backend)
target_link_libraries (
    fillwave
    ${FILLWAVE_MODEL_LOADER}
    ${FILLWAVE_TEXTURE_LOADER}
    fontgenerator
    freetype
    backend)

# -----------------------------------------------
# Test app
# -----------------------------------------------

if (FILLWAVE_BUILD_FREEGLUT_EXAMPLES)
  add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/examples/freeglut)
endif ()

if (FILLWAVE_BUILD_QT_EXAMPLES)
  add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/examples/qt)
endif ()

if (FILLWAVE_BUILD_SDL2_EXAMPLES)
  add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/examples/sdl2)
endif ()

if(FILLWAVE_BUILD_LEVEL_EDITOR)
  add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/tools/level_editor)
endif()

if (FILLWAVE_BUILD_COTIRE)
  include (${FILLWAVE_EXT_COTIRE_PATH}/CMake/cotire.cmake)
  cotire(fillwave)
endif()

# -----------------------------------------------
# Speed up building
# -----------------------------------------------

if (FILLWAVE_BUILD_COTIRE)
  include (${FILLWAVE_EXT_COTIRE_PATH}/CMake/cotire.cmake)
  cotire(fillwave)
endif()
