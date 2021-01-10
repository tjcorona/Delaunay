set(Shape_Test_Files
  UnitTestPoint.cc
  UnitTestPointUtilities.cc
  UnitTestLineSegment.cc
  UnitTestLineSegmentUtilities.cc
  UnitTestTriangle.cc
  UnitTestTriangleUtilities.cc)

create_test_sourcelist(ShapeTestSources ShapeTests.cc ${Shape_Test_Files})
add_executable(ShapeTests ${ShapeTestSources})
target_link_libraries(ShapeTests LINK_PUBLIC
  DelaunayShape DelaunayMisc DelaunayVisualization)

foreach (test ${Shape_Test_Files})
  get_filename_component(tname ${test} NAME_WE)
  add_test(NAME ${tname} COMMAND ShapeTests ${tname})
endforeach(test)
