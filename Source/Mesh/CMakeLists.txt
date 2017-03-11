set(Mesh_Source_Files
  Mesh.cc
  Mesher.cc
  Triangle.cc
)

add_library (DelaunayMesh SHARED ${Mesh_Source_Files})
target_link_libraries (DelaunayMesh DelaunayShape DelaunayMisc)
set_property (TARGET DelaunayMesh PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayMesh DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
