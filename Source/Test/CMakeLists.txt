set (APPLICATIONS)

if (${PROJECT_NAME}_USE_OPENCV)
  find_package(OpenCV REQUIRED)
  list(APPEND CMAKE_INSTALL_RPATH ${OpenCV_INSTALL_PATH}/lib)

  list (APPEND APPLICATIONS Triangle)
  add_executable( Triangle Triangle.cc )
  target_link_libraries(Triangle LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Misc ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS Mesh)
  add_executable( Mesh Mesh.cc )
  target_link_libraries(Mesh LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Misc ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS EnforceMinimumAngle)
  add_executable( EnforceMinimumAngle EnforceMinimumAngle.cc )
  target_link_libraries(EnforceMinimumAngle LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS Function)
  add_executable( Function Function.cc )
  target_link_libraries(Function LINK_PUBLIC
    ${PROJECT_NAME}Misc ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS BezierCurve)
  add_executable(BezierCurve BezierCurve.cc)
  target_link_libraries(BezierCurve LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS Polygon)
  add_executable( Polygon Polygon.cc )
  target_link_libraries(Polygon LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS MeshedPolygon)
  add_executable( MeshedPolygon MeshedPolygon.cc )
  target_link_libraries(MeshedPolygon LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Validation
    ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS MeshedBox)
  add_executable( MeshedBox MeshedBox.cc )
  target_link_libraries(MeshedBox LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS AddEdge)
  add_executable( AddEdge AddEdge.cc )
  target_link_libraries(AddEdge LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS SplitEdge)
  add_executable( SplitEdge SplitEdge.cc )
  target_link_libraries(SplitEdge LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS ExcisePolygon)
  add_executable( ExcisePolygon ExcisePolygon.cc )
  target_link_libraries(ExcisePolygon LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS RandomRefinedPolygon)
  add_executable( RandomRefinedPolygon RandomRefinedPolygon.cc )
  target_link_libraries(RandomRefinedPolygon LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  list (APPEND APPLICATIONS SeditiousEdge)
  add_executable( SeditiousEdge SeditiousEdge.cc )
  target_link_libraries(SeditiousEdge LINK_PUBLIC
    ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

  if (${PROJECT_NAME}_USE_VTK)

    add_executable( vtkPolyDataToPolygon vtkPolyDataToPolygon.cc )
    target_link_libraries(vtkPolyDataToPolygon LINK_PUBLIC
      ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)

    add_executable( ChesapeakeBay ChesapeakeBay.cc )
    target_link_libraries(ChesapeakeBay LINK_PUBLIC ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Discretization ${PROJECT_NAME}Visualization)
    target_compile_definitions(ChesapeakeBay PRIVATE "DATA_DIR=\"${PROJECT_SOURCE_DIR}/data\"")

  endif ()

endif ()

install(TARGETS
        ${APPLICATIONS}
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
