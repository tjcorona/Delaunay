get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

find_package( OpenCV REQUIRED )

set (APPLICATIONS)

set (TRIANGLE_SOURCE
    DTColor.cc
    DTTriangle.cc
    DTCVCanvas.cc
    Triangle.cc
)

list (APPEND APPLICATIONS Triangle)
add_executable( Triangle ${TRIANGLE_SOURCE} )
target_link_libraries( Triangle ${OpenCV_LIBS} )

install (TARGETS ${APPLICATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
