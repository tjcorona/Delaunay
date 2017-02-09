set(Discretization_Source_Files
  AddInteriorPoint.cc
  DiscretizePolygon.cc
  ExcisePolygon.cc
  ExtendMesh.cc
  InsertEdge.cc
  LegalizeEdges.cc
  TestDelaunayCondition.cc)

add_library (DelaunayDiscretization SHARED ${Discretization_Source_Files})
target_link_libraries (DelaunayDiscretization DelaunayShape DelaunayMesh)
set_property (TARGET DelaunayDiscretization PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayDiscretization DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
