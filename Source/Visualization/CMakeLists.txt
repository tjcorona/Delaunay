find_package( OpenCV REQUIRED )

set(Visualization_Source_Files
  CVCanvas.cc
  Color.cc)

add_library (DelaunayVisualization SHARED ${Visualization_Source_Files})
target_link_libraries (DelaunayVisualization DelaunayShape DelaunayMisc
  ${OpenCV_LIBS})
target_include_directories (DelaunayVisualization PUBLIC ${OpenCV_INCLUDE_DIRS})
set_property (TARGET DelaunayVisualization PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayVisualization DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
