add_executable(test-simple sum_test.cc sum.cpp)
target_link_libraries(test-simple gtest gtest_main)