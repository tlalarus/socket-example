//
// Created by minkyung on 23. 3. 28.
//

#include "adapter.hpp"
#include "../model/model_api.h"

using namespace std;

static boost::asio::io_context io_context;

bool Adapter::connect(short port) {

	function<void(eRequestType)> req_handler = bind(&Adapter::handleRequest, this, placeholders::_1);
	unique_ptr<Server> s = unique_ptr<Server>(new Server(io_context, port, req_handler));

	this->server = move(s);

	return true;
}

bool Adapter::run(){
	io_context.run();
}

bool Adapter::sendResponse(sResponseTest response_) {

}

void Adapter::handleRequest(eRequestType code) {

	switch (code) {

		cout <<"Adapter::handleRequest: " << code << endl;
		case eRequestType::REQ_IS_STANDBY: {

			cout << "request from external client: REQ_IS_STANDBY" << endl;
			initEngine();

			// engine에서 adapter로 response 받을 핸들러 등록
			function<void(sResponseTest)> res_handler = bind(&Adapter::sendResponse, this, placeholders::_1);
			registerNotiHandle(res_handler);

			break;
		}

		case eRequestType::REQ_IS_READY: {
			cout << "request from external client: REQ_IS_READY" << endl;

			runEngine();

			break;
		}

		case eRequestType::REQ_IS_TRIGGERED: {

			cout << "request from external client: REQ_IS_TRIGGERED" << endl;

			break;
		}

		case eRequestType::REQ_RESULT: {

			cout << "request from external client: REQ_RESULT" << endl;

			break;
		}

		default: {

			break;
		}
	}

}
