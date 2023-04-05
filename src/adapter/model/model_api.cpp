//
// Created by minkyung on 23. 3. 29.
//

#include <iostream>
#include "model_api.h"
#include "model.hpp"

static Engine* g_engine = nullptr;

using namespace std;
bool initEngine(){

	if(g_engine == nullptr){

		cout << "6) Generate Engine singleton object and Engine::proc func handler to post"  << endl;
		g_engine = Engine::getInstance();

		// 외부 클라이언트로부터 REQ_IS_STANDBY 요청을 받은 상태.
		// 이에 맞는 action code를 넘겨주기
		auto proc_pfunc_in_engine = boost::mem_fn(&Engine::proc);
		function<void(Engine&, int)> task_handle = boost::bind(proc_pfunc_in_engine, boost::placeholders::_1, 0);
		g_engine->postTask(task_handle, 0);
	} else{
		return false;
	}

	return true;

	// adapter의 sendResponse 함수를 콜백등록해 놓는다.
	sResponseTest reply;
	reply.code = static_cast<int32_t>(eResponseType::RES_STANDBY);
	reply.val1 = true;
	g_engine->msg_manager.sendResponse(reply);
}

bool runEngine(){

	cout << "runEngine" << endl;
	g_engine->proc(1);
	g_engine->proc(2);
	g_engine->proc(0);

	sResponseTest reply;
	reply.code = static_cast<int32_t>(eResponseType::RES_READY);
	reply.val1 = true;
	g_engine->msg_manager.sendResponse(reply);

	return true;
}

bool registerNotiHandle(NotiCallbackFn pfunc){

	if(pfunc != nullptr){
		g_engine->msg_manager.registerNotiCB(pfunc);
	} else{
		return false;
	}

	return true;
}
