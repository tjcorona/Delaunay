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
set_property (TARGET ${PROJECT_NAME}Shape PROPERTY MACOSX_RPATH TRUE)

generate_export_header(${PROJECT_NAME}Shape EXPORT_FILE_NAME Export.hh)
list(APPEND Shape_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Shape
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Shape
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(FILES
  ${Shape_Header_Files}
  DESTINATION include/${PROJECT_NAME}/Shape
  )

if (${PROJECT_NAME}_ENABLE_TESTING)
  add_subdirectory(Tests)
endif()
