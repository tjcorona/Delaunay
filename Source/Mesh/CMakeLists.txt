set(Mesh_Header_Files
  Edge.hh
  EdgeSet.hh
  Mesh.hh
  Mesher.hh
  Polygon.hh
  Triangle.hh
  TriangleSet.hh
  Vertex.hh)

set(Mesh_Source_Files
  Mesh.cc
  Mesher.cc
  Polygon.cc
  Triangle.cc
)

add_library (${PROJECT_NAME}Mesh SHARED ${Mesh_Source_Files})
target_link_libraries (${PROJECT_NAME}Mesh ${PROJECT_NAME}Shape
  ${PROJECT_NAME}Misc)

generate_export_header(${PROJECT_NAME}Mesh EXPORT_FILE_NAME Export.hh)
list(APPEND Mesh_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Mesh
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Mesh
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

install(FILES
  ${Mesh_Header_Files}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Mesh
  )
