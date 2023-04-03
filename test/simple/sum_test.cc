// sum_test.cc

#include <iostream>
#include <gtest/gtest.h>
#include "sum.hpp"

// test case name, test name
TEST(SUM_TEST, SUM_TEST_1){
	// sum(1,1) 함수 실행 결과가 2와 동일한지 확인한다 : 동일할 경우 테스트 통과, 다를 경우 테스트 실패
	EXPECT_EQ(2, gtest_example::sum(1,1));
}

int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS(); }