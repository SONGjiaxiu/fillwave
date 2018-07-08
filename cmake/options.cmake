# -----------------------------------------------
# Main Options
# -----------------------------------------------

option (FILLWAVE_BUILD_DEV "Build development package" OFF)
option (FILLWAVE_BUILD_LIB "Build main fillwave package" ON)
option (FILLWAVE_BUILD_DEB "Build deb packages" OFF)
option (FILLWAVE_BUILD_RPM "Build rpm packages" OFF)
option (FILLWAVE_BUILD_TGZ "Build tgz packages" OFF)
option (FILLWAVE_BUILD_PACK "Do not build external packages when not needed (Linux builds)" OFF)

# -----------------------------------------------
# Platform options
# -----------------------------------------------

option (FILLWAVE_COMPILATION_STARTUP_ANIMATION "Run startup animation before any other action" OFF)
option (FILLWAVE_COMPILATION_PC_GLES "Use GLES stub for PC" OFF)
option (FILLWAVE_COMPILATION_OPENGL_4_5 "Support new devices with gl 4.5 support" OFF)
option (FILLWAVE_COMPILATION_DRIVER_WORKAROUNDS "Usable for mesa shader compiler" ON)
option (FILLWAVE_COMPILATION_OPTIMIZE_RAM_USAGE "Asset data are stored only in GPU" ON)
option (FILLWAVE_COMPILATION_OPTIMIZE_ONE_FOCUS "You can now focus only one model at the same time" OFF)
option (FILLWAVE_COMPILATION_SUPPRESS_WARNINGS "Suppress warnings during compilation" OFF)
option (FILLWAVE_COMPILATION_RELEASE "Build type" OFF)

# -----------------------------------------------
# Tests related options
# -----------------------------------------------

option (FILLWAVE_TESTS "Build with gtest tests" OFF)
option (FILLWAVE_TESTS_COVERALLS "Generate coveralls data" OFF)

# -----------------------------------------------
# External libraries options
# -----------------------------------------------

option (FILLWAVE_MODEL_LOADER_ASSIMP "Use Assimp" ON)
option (FILLWAVE_MODEL_LOADER_TINYOBJLOADER "Use Tiny obj loader" OFF)
option (FILLWAVE_TEXTURE_LOADER_CUSTOM "Use custom texture loader" ON)

# -----------------------------------------------
# Build components Options
# -----------------------------------------------

option (FILLWAVE_BUILD_ANDROID_TEST_NATIVE_APP "Build android native test app" OFF)
option (FILLWAVE_BUILD_ANDROID_JNI_LIB "Build android jni library" OFF)
option (FILLWAVE_BUILD_GLFW_EXAMPLES "Build glfw example" OFF)
option (FILLWAVE_BUILD_FREEGLUT_EXAMPLES "Build linux example" OFF)
option (FILLWAVE_BUILD_QT_EXAMPLES "Build qt example" OFF)
option (FILLWAVE_BUILD_LEVEL_EDITOR "Build qt level game editor" OFF)
option (FILLWAVE_BUILD_SDL2_EXAMPLES "Build sdl2 example" OFF)
option (FILLWAVE_BUILD_COTIRE "Build with cotire" OFF)

# -----------------------------------------------
# Benchmark options
# -----------------------------------------------

option (FILLWAVE_BENCHMARK_TESTS "Build binary to test performance" OFF)

# -----------------------------------------------
# Additional Options
# -----------------------------------------------

option (FILLWAVE_DEBUG_OPENGL_APITRACE "Use apitrace to generate OpenGL trace" OFF)

# -----------------------------------------------
# CI Options
# -----------------------------------------------
