set(Visualization_Header_Files
  Color.hh)

set(Visualization_Source_Files
  Color.cc)

if (${PROJECT_NAME}_USE_OPENCV)
  find_package( OpenCV REQUIRED )

  list (APPEND Visualization_Header_Files
    CVCanvas.hh)
  list (APPEND Visualization_Source_Files
    CVCanvas.cc)
endif()

if (${PROJECT_NAME}_USE_VTK)
  find_package(VTK REQUIRED)

  list (APPEND Visualization_Header_Files
    VTKCanvas.hh)
  list (APPEND Visualization_Source_Files
    VTKCanvas.cc)
endif()

add_library (${PROJECT_NAME}Visualization SHARED ${Visualization_Source_Files})
target_link_libraries (${PROJECT_NAME}Visualization
  ${PROJECT_NAME}Shape ${PROJECT_NAME}Misc ${OpenCV_LIBS} ${VTK_LIBRARIES})
target_include_directories (${PROJECT_NAME}Visualization PUBLIC
  ${OpenCV_INCLUDE_DIRS} ${VTK_INCLUDE_DIRS})
set_property (TARGET ${PROJECT_NAME}Visualization PROPERTY MACOSX_RPATH TRUE)

install(TARGETS
        ${PROJECT_NAME}Visualization
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  )

install(FILES
  ${Visualization_Header_Files}
  DESTINATION include/${PROJECT_NAME}/Visualization
  )
