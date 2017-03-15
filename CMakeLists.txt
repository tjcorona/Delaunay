cmake_minimum_required (VERSION 2.8.11)

project (Delaunay)

get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}/install" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# enable c++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
  "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)

IF("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

option (${PROJECT_NAME}_ENABLE_TESTING "Enable testing" OFF)
if (${PROJECT_NAME}_ENABLE_TESTING)
  enable_testing()
endif ()

option (${PROJECT_NAME}_USE_OPENCV "Enables the use of OpenCV" OFF)
option (${PROJECT_NAME}_USE_VTK "Enables the use of VTK" OFF)

include(GenerateExportHeader)

include_directories (Source)
add_subdirectory (Source)
