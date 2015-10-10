get_filename_component(BUILD_PARENT_DIR ${CMAKE_BINARY_DIR} PATH)
set(CMAKE_INSTALL_PREFIX "${BUILD_PARENT_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

find_package( OpenCV REQUIRED )

set (APPLICATIONS)

set (TRIANGLE_SOURCE
    DTColor.cc
    DTTriangle.cc
    DTCVCanvas.cc
    Random.cc
    Triangle.cc
)

list (APPEND APPLICATIONS Triangle)
add_executable( Triangle ${TRIANGLE_SOURCE} )
target_link_libraries( Triangle ${OpenCV_LIBS} )

set (MESH_SOURCE
    DTColor.cc
    DTTriangle.cc
    DTCVCanvas.cc
    Random.cc
    DTMesh.cc
    DTDelaunayMesh.cc
    Mesh.cc
)

list (APPEND APPLICATIONS Mesh)
add_executable( Mesh ${MESH_SOURCE} )
target_link_libraries( Mesh ${OpenCV_LIBS} )

set (FUNCTION_SOURCE
    DTColor.cc
    DTQuadraticFunction.cc
    DTCVCanvas.cc
    Function.cc
)

list (APPEND APPLICATIONS Function)
add_executable( Function ${FUNCTION_SOURCE} )
target_link_libraries( Function ${OpenCV_LIBS} )

set (BEZIER_SOURCE
    DTColor.cc
    DTBezierCurve.cc
    DTCVCanvas.cc
    Random.cc
    BezierCurve.cc
)

list (APPEND APPLICATIONS BezierCurve)
add_executable( BezierCurve ${BEZIER_SOURCE} )
target_link_libraries( BezierCurve ${OpenCV_LIBS} )

install (TARGETS ${APPLICATIONS} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
