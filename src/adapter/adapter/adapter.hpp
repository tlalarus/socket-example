//
// Created by minkyung on 23. 3. 28.
//

#ifndef SOCKET_ADAPTER_HPP
#define SOCKET_ADAPTER_HPP

#include <iostream>
#include "async_server.hpp"
#include "../include/protocol.h"

class Adapter {
public:
	Adapter() = default;
	~Adapter() = default;

	bool connect(short port);
	bool run();
	bool destroy();

	// server(engine) -> client(adapter):
	// engine에서 보내준 response 종류에 따라 처리함.
	bool sendResponse();

	// client(external) -> server(adapter):
	// server 멤버변수에서 request가 발생하면 콜백으로 불려진다.
	// 외부 클라이언트에서 요청한 request 종류에 따른 엔진메소드를 호출함.
	void handleRequest(eRequestType code);

	//
	void receiveResponse(sResponseTest res);

private:

	std::unique_ptr<Server> server;
};


#endif //SOCKET_ADAPTER_HPP
