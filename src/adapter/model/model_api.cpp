//
// Created by minkyung on 23. 3. 29.
//

#include <iostream>
#include "model_api.h"
#include "model.hpp"

static Engine* g_engine = nullptr;

using namespace std;
bool initEngine(){

	cout << "initEngine" << endl;
	if(g_engine == nullptr){
		g_engine = Engine::getInstance();
	} else{
		return false;
	}

	return true;
}

bool runEngine(){

	cout << "runEngine" << endl;
	g_engine->proc(1);
	g_engine->proc(2);
	g_engine->proc(0);

	return true;
}

bool registerHandle(NotiCallbackFn pfunc){

	if(pfunc != nullptr){
		g_engine->msg_manager.registerNotiCB(pfunc);
	} else{
		return false;
	}

	return true;
}
