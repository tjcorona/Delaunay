set(Validation_Header_Files
  IsValidPolygon.hh)

set(Validation_Source_Files
  IsValidPolygon.cc)

add_library (${PROJECT_NAME}Validation SHARED ${Validation_Source_Files})
target_link_libraries (${PROJECT_NAME}Validation
  ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Visualization)

generate_export_header(${PROJECT_NAME}Validation EXPORT_FILE_NAME Export.hh)
list(APPEND Validation_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Validation
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

install(TARGETS
        ${PROJECT_NAME}Validation
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(FILES
  ${Validation_Header_Files}
  DESTINATION include/${PROJECT_NAME}/Validation
  )
