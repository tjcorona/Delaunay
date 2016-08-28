set(Mesh_Source_Files
#  Polygon.cc
  Triangle.cc
)

add_library (DelaunayMesh SHARED ${Mesh_Source_Files})
target_link_libraries (DelaunayMesh DelaunayShape)
set_property (TARGET DelaunayMesh PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayMesh DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
