//
// Created by minkyung on 23. 3. 29.
//

#include <iostream>
#include "model_api.h"
#include "adapter.hpp"

using namespace std;

void testModel(){
	initEngine();
	std::function<void(eResponseType, struct _payload)> noti =
					[](eResponseType type, struct _payload pl){
		cout << "Responsetype: " << static_cast<int>(type);
		cout << " Data: " << pl.val << endl;

	};

	registerHandle(noti);

	runEngine();
}

void runAdapter(){
	Adapter adapter;
	adapter.connect(4000);
	adapter.run();
}
int main(){
//	testModel();
	runAdapter();
}


