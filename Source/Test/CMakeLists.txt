set (APPLICATIONS)

list (APPEND APPLICATIONS Triangle)
add_executable( Triangle Triangle.cc )
target_link_libraries(Triangle LINK_PUBLIC
  DelaunayShape DelaunayMisc DelaunayVisualization)

#list (APPEND APPLICATIONS Mesh)
#add_executable( Mesh Mesh.cc )
#target_link_libraries(Mesh LINK_PUBLIC
#  DelaunayShape DelaunayMisc DelaunayDiscretization DelaunayVisualization)

list (APPEND APPLICATIONS Function)
add_executable( Function Function.cc )
target_link_libraries(Function LINK_PUBLIC
  DelaunayMisc DelaunayVisualization)

list (APPEND APPLICATIONS BezierCurve)
add_executable(BezierCurve BezierCurve.cc)
target_link_libraries(BezierCurve LINK_PUBLIC
  DelaunayShape DelaunayVisualization)

list (APPEND APPLICATIONS Polygon)
add_executable( Polygon Polygon.cc )
target_link_libraries(Polygon LINK_PUBLIC
  DelaunayShape DelaunayVisualization)

list (APPEND APPLICATIONS MeshedPolygon)
add_executable( MeshedPolygon MeshedPolygon.cc )
target_link_libraries(MeshedPolygon LINK_PUBLIC
  DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

install (TARGETS ${APPLICATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
