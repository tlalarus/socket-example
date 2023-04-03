//
// Created by minkyung on 23. 3. 29.
//

#include "model.hpp"

using namespace std;

Engine* Engine::instance = nullptr;
NotiCallbackFn Message::func_response = nullptr;

void Model::notifyError(Engine &engine_) {
	string msg = "error occured";
	sResponseTest res_msg;
	res_msg.code = static_cast<int32_t>(eResponseType::RES_ERROR);
	res_msg.val1 = false;
	res_msg.val2 = 0.f; res_msg.val3 = 0.f;
	engine_.msg_manager.sendResponse(res_msg);
}

Engine::Engine(){
	cout << "Endgine constructor()" << endl;
	unique_ptr<Model> model(new Model);
	m_model = move(model);
}

void Engine::proc(int action){
	if(action == 1){
		m_model->do_something1(*this);
	} else if(action == 2){
		m_model->do_something2(*this);
	} else{
		m_model->notifyError(*this);
	}
}
