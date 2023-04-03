//
// Created by minkyung on 23. 3. 29.
//

#include <iostream>
#include "adapter/adapter.hpp"

using namespace std;


void runAdapter(){
	Adapter adapter;
	adapter.connect(4000);
	adapter.run();
}
int main(){
//	testModel();
	runAdapter();
}


