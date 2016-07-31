set(Shape_Source_Files
  BezierCurve.cc
  Polygon.cc
  LineSegment.cc
  Triangle.cc)

add_library (DelaunayShape SHARED ${Shape_Source_Files})
#target_include_directories (DelaunayShape PUBLIC
#  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_property (TARGET DelaunayShape PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayShape DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
