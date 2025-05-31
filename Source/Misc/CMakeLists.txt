set(Misc_Header_Files
  Function.hh
  QuadraticFunction.hh
  Random.hh)

set(Misc_Source_Files
  QuadraticFunction.cc
  Random.cc)

add_library (${PROJECT_NAME}Misc SHARED ${Misc_Source_Files})

generate_export_header(${PROJECT_NAME}Misc EXPORT_FILE_NAME Export.hh)
list(APPEND Misc_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Misc
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Misc
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Runtime
  )

install(FILES
  ${Misc_Header_Files}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/Misc
  COMPONENT Development
  )
