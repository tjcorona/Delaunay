set(Misc_Header_Files
  Function.hh
  QuadraticFunction.hh
  Random.hh)

set(Misc_Source_Files
  QuadraticFunction.cc
  Random.cc)

add_library (${PROJECT_NAME}Misc SHARED ${Misc_Source_Files})
set_property (TARGET ${PROJECT_NAME}Misc PROPERTY MACOSX_RPATH TRUE)

target_include_directories(${PROJECT_NAME}Misc
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Misc
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(FILES
  ${Misc_Header_Files}
  DESTINATION include/${PROJECT_NAME}/Misc
  )
