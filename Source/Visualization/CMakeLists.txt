set(Visualization_Header_Files
  Canvas.hh
  Color.hh)

set(Visualization_Source_Files
  Canvas.cc
  Color.cc)

if (${PROJECT_NAME}_USE_OPENCV)
  find_package(OpenCV REQUIRED)
  set(_opencv_libs ${OpenCV_LIBS})
  set(_opencv_inc_dirs ${OpenCV_INCLUDE_DIRS})

  list(APPEND CMAKE_INSTALL_RPATH ${OpenCV_INSTALL_PATH}/lib)

  list (APPEND Visualization_Header_Files
    CVCanvas.hh)
  list (APPEND Visualization_Source_Files
    CVCanvas.cc)
endif()

if (${PROJECT_NAME}_USE_VTK)
  find_package(VTK REQUIRED COMPONENTS
    CommonCore
    CommonDataModel
    FiltersSources
    RenderingCore
    RenderingOpenGL2
    IOXML
    InteractionStyle)
  set(_vtk_libs ${VTK_LIBRARIES})
  set(_vtk_inc_dirs ${VTK_INCLUDE_DIRS})

  list (APPEND Visualization_Header_Files
    VTKCanvas.hh)
  list (APPEND Visualization_Source_Files
    VTKCanvas.cc)
endif()

add_library (${PROJECT_NAME}Visualization SHARED ${Visualization_Source_Files})
target_link_libraries (${PROJECT_NAME}Visualization
  ${PROJECT_NAME}Shape ${PROJECT_NAME}Mesh ${PROJECT_NAME}Misc ${_opencv_libs} ${_vtk_libs})
target_include_directories (${PROJECT_NAME}Visualization PUBLIC
  ${_opencv_inc_dirs} ${_vtk_inc_dirs})

generate_export_header(${PROJECT_NAME}Visualization EXPORT_FILE_NAME Export.hh)
list(APPEND Visualization_Header_Files ${CMAKE_CURRENT_BINARY_DIR}/Export.hh)

target_include_directories(${PROJECT_NAME}Visualization
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Source/>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

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
