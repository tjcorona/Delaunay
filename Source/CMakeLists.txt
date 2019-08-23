add_subdirectory(Discretization)
add_subdirectory(Mesh)
add_subdirectory(Misc)
add_subdirectory(Shape)
add_subdirectory(Test)
add_subdirectory(Validation)
add_subdirectory(Visualization)

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/CMake/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-Config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  )

export(EXPORT ${PROJECT_NAME}
  FILE ${PROJECT_NAME}-targets.cmake
  )

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-Config.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )
