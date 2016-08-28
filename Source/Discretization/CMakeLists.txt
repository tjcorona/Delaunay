set(Discretization_Source_Files
  Mesh.cc
  DelaunayMesh.cc)

add_library (DelaunayDiscretization SHARED ${Discretization_Source_Files})
target_link_libraries (DelaunayDiscretization DelaunayShape)
set_property (TARGET DelaunayDiscretization PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayDiscretization DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
