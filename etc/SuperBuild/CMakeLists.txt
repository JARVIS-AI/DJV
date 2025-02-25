cmake_minimum_required(VERSION 3.18)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0097 NEW)

project(DJV-SuperBuild)

list(PREPEND CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/../../cmake/Modules
    ${PROJECT_SOURCE_DIR}/cmake/Modules)

set(DJV_THIRD_PARTY_OPTIONAL TRUE CACHE BOOL "Use optional third party dependencies")
set(DJV_THIRD_PARTY_Blosc FALSE CACHE BOOL "Build Blosc")
set(DJV_THIRD_PARTY_TBB FALSE CACHE BOOL "Build TBB")
set(DJV_THIRD_PARTY_Ptex FALSE CACHE BOOL "Build Ptex")
set(DJV_THIRD_PARTY_HDF5 FALSE CACHE BOOL "Build HDF5")
set(DJV_THIRD_PARTY_Boost FALSE CACHE BOOL "Build Boost")
set(DJV_THIRD_PARTY_ISPC FALSE CACHE BOOL "Build ISPC")
set(DJV_THIRD_PARTY_curl FALSE CACHE BOOL "Build curl")
set(DJV_THIRD_PARTY_FFmpeg FALSE CACHE BOOL "Build FFmpeg")
set(DJV_THIRD_PARTY_JPEG TRUE CACHE BOOL "Build JPEG")
set(DJV_THIRD_PARTY_TIFF TRUE CACHE BOOL "Build TIFF")
set(DJV_THIRD_PARTY_OpenEXR TRUE CACHE BOOL "Build OpenEXR")
set(DJV_THIRD_PARTY_OpenVDB FALSE CACHE BOOL "Build OpenVDB")
set(DJV_THIRD_PARTY_OpenCV FALSE CACHE BOOL "Build OpenCV")
set(DJV_THIRD_PARTY_OpenImageIO FALSE CACHE BOOL "Build OpenImageIO")
set(DJV_THIRD_PARTY_OpenImageDenoise FALSE CACHE BOOL "Build OpenImageDenoise")
set(DJV_THIRD_PARTY_OpenNURBS FALSE CACHE BOOL "Build OpenNURBS")
set(DJV_THIRD_PARTY_embree FALSE CACHE BOOL "Build embree")
set(DJV_THIRD_PARTY_rkcommon FALSE CACHE BOOL "Build rkcommon")
set(DJV_THIRD_PARTY_OpenVKL FALSE CACHE BOOL "Build OpenVKL")
set(DJV_THIRD_PARTY_ospray FALSE CACHE BOOL "Build ospray")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>DLL)

enable_testing()

if(APPLE)
    set(CMAKE_CXX_STANDARD 11)
endif()

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

# Add Python dependency.
if(DJV_PYTHON)
    find_package(Python)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG v2.3.0)
    FetchContent_GetProperties(pybind11)
    if(NOT pybind11_POPULATED)
        FetchContent_Populate(pybind11)
        add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
    endif()
endif()

# Add required dependencies.
set(DJV_THIRD_PARTY_DEPS)
list(APPEND DJV_THIRD_PARTY_DEPS ZLIB)
list(APPEND DJV_THIRD_PARTY_DEPS FSeq)
list(APPEND DJV_THIRD_PARTY_DEPS glad)
list(APPEND DJV_THIRD_PARTY_DEPS GLM)
list(APPEND DJV_THIRD_PARTY_DEPS GLFW)
list(APPEND DJV_THIRD_PARTY_DEPS RapidJSON)
#list(APPEND DJV_THIRD_PARTY_DEPS OpenAL)
list(APPEND DJV_THIRD_PARTY_DEPS RtAudio)
#list(APPEND DJV_THIRD_PARTY_DEPS Imath)
list(APPEND DJV_THIRD_PARTY_DEPS IlmBase)
list(APPEND DJV_THIRD_PARTY_DEPS PNG)
list(APPEND DJV_THIRD_PARTY_DEPS OCIO)
#list(APPEND DJV_THIRD_PARTY_DEPS OTIO)
list(APPEND DJV_THIRD_PARTY_DEPS FreeType)

# Add optional dependencies.
if(DJV_THIRD_PARTY_Blosc)
    list(APPEND DJV_THIRD_PARTY_DEPS Blosc)
endif()
if(DJV_THIRD_PARTY_TBB)
    list(APPEND DJV_THIRD_PARTY_DEPS TBB)
endif()
if(DJV_THIRD_PARTY_Ptex)
    list(APPEND DJV_THIRD_PARTY_DEPS Ptex)
endif()
if(DJV_THIRD_PARTY_HDF5)
    list(APPEND DJV_THIRD_PARTY_DEPS HDF5)
endif()
if(DJV_THIRD_PARTY_Boost)
    list(APPEND DJV_THIRD_PARTY_DEPS Boost)
endif()
if(DJV_THIRD_PARTY_ISPC)
    list(APPEND DJV_THIRD_PARTY_DEPS ISPC)
endif()
if(DJV_THIRD_PARTY_curl)
    if(WIN32)
    elseif(APPLE)
    else()
        list(APPEND DJV_THIRD_PARTY_DEPS MbedTLS)
    endif()
    list(APPEND DJV_THIRD_PARTY_DEPS curl)
endif()
if(DJV_THIRD_PARTY_FFmpeg)
    list(APPEND DJV_THIRD_PARTY_DEPS NASM)
    list(APPEND DJV_THIRD_PARTY_DEPS FFmpeg)
endif()
if(DJV_THIRD_PARTY_JPEG)
    if(NOT WIN32 AND NOT NASM IN_LIST DJV_THIRD_PARTY_DEPS)
        list(APPEND DJV_THIRD_PARTY_DEPS NASM)
    endif()
    list(APPEND DJV_THIRD_PARTY_DEPS JPEG)
endif()
if(DJV_THIRD_PARTY_TIFF)
    list(APPEND DJV_THIRD_PARTY_DEPS TIFF)
endif()
if(DJV_THIRD_PARTY_OpenEXR)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenEXR)
endif()
if(DJV_THIRD_PARTY_OpenVDB)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenVDB)
endif()
if(DJV_THIRD_PARTY_OpenCV)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenCV)
endif()
if(DJV_THIRD_PARTY_OpenImageIO)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenImageIO)
endif()
if(DJV_THIRD_PARTY_OpenImageDenoise)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenImageDenoise)
endif()
if(DJV_THIRD_PARTY_OpenNURBS)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenNURBS)
endif()
if(DJV_THIRD_PARTY_embree)
    list(APPEND DJV_THIRD_PARTY_DEPS embree)
endif()
if(DJV_THIRD_PARTY_rkcommon)
    list(APPEND DJV_THIRD_PARTY_DEPS rkcommon)
endif()
if(DJV_THIRD_PARTY_OpenVKL)
    list(APPEND DJV_THIRD_PARTY_DEPS OpenVKL)
endif()
if(DJV_THIRD_PARTY_ospray)
    list(APPEND DJV_THIRD_PARTY_DEPS ospray)
endif()

# Build dependencies.
foreach(DEP ${DJV_THIRD_PARTY_DEPS})
    include(Build${DEP})
endforeach()

# Add tests.
include(Buildtests)

# Add DJV.
include(BuildDJV)
