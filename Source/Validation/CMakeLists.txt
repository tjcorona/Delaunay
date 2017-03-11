set(Validation_Source_Files
  IsValidPolygon.cc)

add_library (DelaunayValidation SHARED ${Validation_Source_Files})
target_link_libraries (DelaunayValidation DelaunayShape DelaunayMesh DelaunayVisualization)
set_property (TARGET DelaunayValidation PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayValidation DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
