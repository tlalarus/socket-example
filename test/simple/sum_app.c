// sum_app.c

#include <stdio.h>
#include "sum.h"

using namespace std;
int main()
{
	int val = gtest_example::sum(1, 2);
	printf("sum_app: %d\n", val);
}