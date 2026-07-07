cmake_minimum_required(VERSION 3.15)
project(pyMMVII LANGUAGES CXX)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

# strict C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Position Independent Code for python binding
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/sources")

# PROJ
find_package(PROJ QUIET)
if (NOT PROJ_FOUND)
  # needed on Ubuntu
  find_package(PkgConfig QUIET)
  if(PKG_CONFIG_FOUND)
    pkg_search_module(PROJ REQUIRED proj)
  endif()

  # Fallback
  if (NOT PROJ_FOUND AND DEFINED ENV{CONDA_PREFIX})
    set(PROJ_INCLUDE_DIRS "$ENV{CONDA_PREFIX}/Library/include")
    set(PROJ_LIBRARIES "$ENV{CONDA_PREFIX}/Library/lib/proj.lib")
    set(PROJ_FOUND TRUE)
  endif()
endif()

if (PROJ_FOUND AND CMAKE_HOST_WIN32)
  find_file(PROJ_PROJ_DB_FILE proj.db PATHS ${PROJ_DIR} ${PROJ_BINARY_DIRS}/../share/proj NO_DEFAULT_PATH)
  file(COPY ${PROJ_PROJ_DB_FILE} DESTINATION ${MMVII_SOURCE_DIR}/share/proj)
endif()

# GDAL
find_package(GDAL 3.4 REQUIRED)

# Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

# OpenMP: Optional, included in GNU C++, separate package for CLANG.
find_package(OpenMP)
if((NOT OpenMP_CXX_FOUND) AND (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
  message(STATUS "OpenMP not found: did you install libomp-dev ?")
endif()

# compilers specific options : warnings, MSVC specific defines
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(gnu_cxx  "$<COMPILE_LANG_AND_ID:CXX,GNU>")

set(mmv2_src_dir "${PROJECT_SOURCE_DIR}/sources/src")
set(mmv2_include_dir "${PROJECT_SOURCE_DIR}/sources/include" "${PROJECT_SOURCE_DIR}/sources")
set(mmv2_lib $<TARGET_OBJECTS:P2007>)

set(mmv2_external_include_dir "${PROJECT_SOURCE_DIR}/sources/ExternalInclude")
set(EIGEN3_INCLUDE_PATH  "${mmv2_external_include_dir}/eigen-3.4.0")
set(libE57Format_lib "${PROJECT_SOURCE_DIR}/sources/bin/libE57Format.a")

file(GLOB mmv2_apipy_srcs CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB mmv2_headers "${mmv2_include_dir}/*.h" "${mmv2_include_dir}/*/*.h" "${mmv2_src_dir}/*/*.h")

# Debug output to check if files are found
message(STATUS "mmv2_apipy_srcs: ${mmv2_apipy_srcs}")

# Create the pybind11 module
pybind11_add_module(_MMVII "${mmv2_apipy_srcs}")

# Specify include directories and link libraries for the target
target_include_directories(_MMVII PRIVATE "${mmv2_include_dir}" "${EIGEN3_INCLUDE_PATH}" "${PROJ_INCLUDE_DIRS}" "${GDAL_INCLUDE_DIRS}" "${PROJECT_SOURCE_DIR}")
target_link_libraries(_MMVII PRIVATE Threads::Threads ${PROJ_LIBRARIES} ${GDAL_LIBRARIES} ${mmv2_lib} ${libE57Format_lib})

# Force parallel compilation with MSVC
if(MSVC)
 target_compile_options(_MMVII PRIVATE "/MP")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1")
  target_link_libraries(_MMVII PRIVATE -lstdc++fs)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  target_link_libraries(_MMVII PRIVATE -lc++fs)
endif()

if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found")
    target_link_libraries(_MMVII PRIVATE OpenMP::OpenMP_CXX)
    target_compile_options(_MMVII PRIVATE ${OpenMP_CXX_FLAGS})
    target_link_options(_MMVII PRIVATE ${OpenMP_EXE_LINKER_FLAGS})
endif()

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(_MMVII PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_link_options(_MMVII PRIVATE -lproj -lgdal -lxerces-c)

add_custom_command(
  TARGET _MMVII
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/sources/bin" "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

