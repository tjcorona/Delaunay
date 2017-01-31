set(Misc_Source_Files
  QuadraticFunction.cc
  Random.cc)

add_library (DelaunayMisc SHARED ${Misc_Source_Files})
set_property (TARGET DelaunayMisc PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayMisc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
