set(Misc_Source_Files
  QuadraticFunction.cc
  Random.cc)

add_library (DelaunayMisc SHARED ${Misc_Source_Files})
target_include_directories (DelaunayMisc PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
set_property (TARGET DelaunayMisc PROPERTY MACOSX_RPATH TRUE)

install (TARGETS DelaunayMisc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
