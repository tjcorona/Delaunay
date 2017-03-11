find_package( OpenCV REQUIRED )

set(Visualization_Source_Files
  CVCanvas.cc
  Color.cc)

if (DELAUNAY_USE_VTK)
  find_package(VTK REQUIRED)

  list (APPEND Visualization_Source_Files
    VTKCanvas.cc)
endif()

add_library (DelaunayVisualization SHARED ${Visualization_Source_Files})
target_link_libraries (DelaunayVisualization DelaunayShape DelaunayMisc
  ${OpenCV_LIBS} ${VTK_LIBRARIES})
target_include_directories (DelaunayVisualization PUBLIC ${OpenCV_INCLUDE_DIRS} ${VTK_INCLUDE_DIRS})
set_property (TARGET DelaunayVisualization PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayVisualization DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
