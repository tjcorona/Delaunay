set (APPLICATIONS)

if (DELAUNAY_USE_OPENCV)

  list (APPEND APPLICATIONS Triangle)
  add_executable( Triangle Triangle.cc )
  target_link_libraries(Triangle LINK_PUBLIC
    DelaunayShape DelaunayMisc DelaunayVisualization)

  list (APPEND APPLICATIONS Mesh)
  add_executable( Mesh Mesh.cc )
  target_link_libraries(Mesh LINK_PUBLIC
    DelaunayShape DelaunayMisc DelaunayDiscretization DelaunayVisualization)

  list (APPEND APPLICATIONS EnforceMinimumAngle)
  add_executable( EnforceMinimumAngle EnforceMinimumAngle.cc )
  target_link_libraries(EnforceMinimumAngle LINK_PUBLIC
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

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
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayValidation
    DelaunayVisualization)

  list (APPEND APPLICATIONS MeshedBox)
  add_executable( MeshedBox MeshedBox.cc )
  target_link_libraries(MeshedBox LINK_PUBLIC
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

  list (APPEND APPLICATIONS AddEdge)
  add_executable( AddEdge AddEdge.cc )
  target_link_libraries(AddEdge LINK_PUBLIC
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

  list (APPEND APPLICATIONS SplitEdge)
  add_executable( SplitEdge SplitEdge.cc )
  target_link_libraries(SplitEdge LINK_PUBLIC
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

  list (APPEND APPLICATIONS ExcisePolygon)
  add_executable( ExcisePolygon ExcisePolygon.cc )
  target_link_libraries(ExcisePolygon LINK_PUBLIC
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

  list (APPEND APPLICATIONS RandomRefinedPolygon)
  add_executable( RandomRefinedPolygon RandomRefinedPolygon.cc )
  target_link_libraries(RandomRefinedPolygon LINK_PUBLIC
    DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

  if (DELAUNAY_USE_VTK)

    add_executable( vtkPolyDataToPolygon vtkPolyDataToPolygon.cc )
    target_link_libraries(vtkPolyDataToPolygon LINK_PUBLIC
      DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)

    add_executable( ChesapeakeBay ChesapeakeBay.cc )
    target_link_libraries(ChesapeakeBay LINK_PUBLIC DelaunayShape DelaunayMesh DelaunayDiscretization DelaunayVisualization)
    target_compile_definitions(ChesapeakeBay PRIVATE "DELAUNAY_DATA_DIR=\"${PROJECT_SOURCE_DIR}/data\"")

  endif ()

endif ()

install (TARGETS ${APPLICATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
