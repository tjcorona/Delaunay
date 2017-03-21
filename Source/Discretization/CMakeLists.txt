set(Discretization_Header_Files
  AddInteriorPoint.hh
  ConstrainedDelaunayMesh.hh
  CutEdge.hh
  DiscretizePolygon.hh
  EnforceMinimumAngle.hh
  ExcisePolygon.hh
  ExtendMesh.hh
  InsertLineSegment.hh
  LegalizeEdges.hh
  RemoveBoundedRegion.hh
  RuppertsAlgorithm.hh
  SplitEdge.hh
  SplitTriangle.hh
  TestDelaunayCondition.hh)

set(Discretization_Source_Files
  AddInteriorPoint.cc
  ConstrainedDelaunayMesh.cc
  CutEdge.cc
  DiscretizePolygon.cc
  EnforceMinimumAngle.cc
  ExcisePolygon.cc
  ExtendMesh.cc
  InsertLineSegment.cc
  LegalizeEdges.cc
  RemoveBoundedRegion.cc
  RuppertsAlgorithm.cc
  SplitEdge.cc
  SplitTriangle.cc
  TestDelaunayCondition.cc)

add_library (${PROJECT_NAME}Discretization SHARED ${Discretization_Source_Files})
target_link_libraries (${PROJECT_NAME}Discretization
  ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Visualization)

generate_export_header(${PROJECT_NAME}Discretization EXPORT_FILE_NAME Export.hh)
list(APPEND Discretization_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Discretization
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Discretization
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(FILES
  ${Discretization_Header_Files}
  DESTINATION include/${PROJECT_NAME}/Discretization
  )
