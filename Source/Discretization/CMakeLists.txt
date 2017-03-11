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

add_library (DelaunayDiscretization SHARED ${Discretization_Source_Files})
target_link_libraries (DelaunayDiscretization DelaunayShape DelaunayMesh DelaunayVisualization)
set_property (TARGET DelaunayDiscretization PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayDiscretization DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
