cmake_minimum_required(VERSION 3.0)

project(GeoSketch)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MACOSX_RPATH 1)

# optional configuration
option(BUILD_UNIT_TESTS "Build unit tests (default OFF)" OFF)
option(SHARED_SFML "Use shared SFML libraries (default OFF)" ON)

# # initialize conan libs
# include(CMake/conan.cmake)
# conan_cmake_run(REQUIRES
#     boost/1.77.0
#     ogg/1.3.5
#     sfml/2.5.1
#     spdlog/1.9.2
#     nlohmann_json/3.10.5
#     zlib/1.2.12
#     BASIC_SETUP CMAKE_TARGETS
#     GENERATORS cmake_find_package cmake_paths
#     BUILD missing
#     OPTIONS 
#         boost:shared=False 
#         boost:without_test=False 
#         boost:without_program_options=False 
#         boost:without_filesystem=False 
#         boost:without_system=False 
#         boost:without_exception=False
#         boost:without_contract=False
#         sfml:shared=True
# )
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS KEEP_RPATHS)

# Add a RELEASE definition that we can count on regardless
# of platform
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -DRELEASE")
string(APPEND CMAKE_CXX_FLAGS_MINSIZEREL " -DRELEASE")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost COMPONENTS)
include_directories(${Boost_INCLUDE_DIRS})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

if (MSVC)
    add_definitions(
        -DWIN32
        -D_WIN32
        -D_WINDOWS
        -D_WIN32_WINDOWS
        -DNOMINMAX
        -D_SCL_SECURE_NO_WARNINGS
        -DWIN32_LEAN_AND_MEAN
        -D_HAS_ITERATOR_DEBUGGING=0

        # Windows "warning C4996" deprecation warnings
        -D_SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING
        -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
    )

    add_compile_options(
        # treat warnings as errors
        /WX
    )

    add_link_options(
        # eliminate a lot of debug linking errors
        /IGNORE:4099
    )
endif(MSVC)

# Add a RELEASE definition that we can count on regardless
# of platform
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -DRELEASE")
string(APPEND CMAKE_CXX_FLAGS_MINSIZEREL " -DRELEASE")


# include TGUI and build only what we need
set(TGUI_BUILD_GUI_BUILDER FALSE)
set(TGUI_BUILD_EXAMPLES FALSE)
set(TGUI_BUILD_TESTS FALSE)
set(TGUI_STATIC_LIBRARIES TRUE)
set(TGUI_SHARED_LIBS FALSE)
set(TGUI_BUILD_FRAMEWORK FALSE)
add_subdirectory(TGUI)
include_directories(TGUI/include)

add_subdirectory(src)

if (BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
