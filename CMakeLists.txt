if (CMAKE_CURRENT_LIST_DIR STREQUAL CMAKE_SOURCE_DIR)
  cmake_minimum_required (VERSION 3.0)
endif()

project (Delaunay)

include(GNUInstallDirs)

# if the project is being built by itself, set the install path to be inside the
# source directory
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
  set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}/install" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif ()

# disable rpath
if (NOT DEFINED CMAKE_INSTALL_NAME_DIR)
  set(CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()

# enable c++11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS FALSE)

if (NOT DEFINED CMAKE_INSTALL_CMAKEDIR)
  set(CMAKE_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
endif ()

if (NOT DEFINED "${PROJECT_NAME}_ENABLE_TESTING")
  option (${PROJECT_NAME}_ENABLE_TESTING "Enable testing" OFF)
endif ()
if (${PROJECT_NAME}_ENABLE_TESTING)
  enable_testing()
endif ()

if (NOT DEFINED "${PROJECT_NAME}_USE_OPENCV")
  option (${PROJECT_NAME}_USE_OPENCV "Enables the use of OpenCV" OFF)
endif ()
if (NOT DEFINED "${PROJECT_NAME}_USE_VTK")
  option (${PROJECT_NAME}_USE_VTK "Enables the use of VTK" OFF)
endif ()

include(GenerateExportHeader)

include_directories (Source)
add_subdirectory (Source)
