set(Shape_Header_Files
  BezierCurve.hh
  Circle.hh
  CircleUtilities.hh
  LineSegment.hh
  LineSegmentUtilities.hh
  ParametricCurve.hh
  Point.hh
  PointUtilities.hh
  Polygon.hh
  PolygonUtilities.hh
  Triangle.hh
  TriangleUtilities.hh)

set(Shape_Source_Files
  BezierCurve.cc
  CircleUtilities.cc
  PolygonUtilities.cc
  PointUtilities.cc
  LineSegment.cc
  LineSegmentUtilities.cc
  Triangle.cc
  TriangleUtilities.cc)

add_library (${PROJECT_NAME}Shape SHARED ${Shape_Source_Files})

generate_export_header(${PROJECT_NAME}Shape EXPORT_FILE_NAME Export.hh)
list(APPEND Shape_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Shape
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Shape
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

install(FILES
  ${Shape_Header_Files}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Shape
  )

if (${PROJECT_NAME}_ENABLE_TESTING)
  add_subdirectory(Tests)
endif()
