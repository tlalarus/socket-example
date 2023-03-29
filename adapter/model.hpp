//
// Created by minkyung on 23. 3. 29.
//

#ifndef SOCKET_MODEL_HPP
#define SOCKET_MODEL_HPP

#include <stdio.h>
#include <iostream>
#include <cstring>
#include <memory>
#include <mutex>
#include "protocol.h"


//#include <functional>
//using CallbackFn = std::function<void(eResponseType code, struct _payload payloads)>;

// Server
class Message{
public:
	Message() {};
	~Message(){};

	// response가 준비 되었을때 Adapter 에게 notify할 콜백함수 등록하는 함수
	static void registerNotiCB(CallbackFn pfunc) {
		func_response = std::bind(pfunc, std::placeholders::_1, std::placeholders::_2);
	};

	// server(engine)에서 request를 받는다.
	bool handleRequest(){
		bool res = false;

		while(true){
			if(req_msg.code > 0){
				std::cout << "Server received request" << std::endl;
				res = true;
				break;
			}
		}

		return res;
	};

	// server(engine)에서 response를 보낸다.
	void sendResponse(eResponseType response_type, std::string noti_msg){
		payload pl;
		std::size_t msgsize = noti_msg.size();

		pl.val = new uint8_t[msgsize];
		pl.length = msgsize;

		memcpy(pl.val, noti_msg.c_str(), msgsize);

		// send
		func_response(response_type, pl);

		delete pl.val;
	};

private:
	static CallbackFn func_response;
	static sRequest req_msg;

};

class Engine;

class Model{
public:
	Model() = default;
	~Model() = default;

	void do_something1(Engine& engine_) { std::cout << "do_something1()" << std::endl; };
	void do_something2(Engine& engine_) { std::cout << "do_something2()" << std::endl; };
	void notifyError(Engine& engine_);
};

//TODO
// convert Thread-safe singleton object
class Engine{
public:
	~Engine() = default;

	Engine(const Engine &) = delete;
	Engine(Engine &&) = delete;
	Engine &operator=(const Engine &) = delete;
	Engine &operator=(Engine &&) = delete;

	Message msg_manager;

	void proc(int action);
private:
	static Engine* instance;
	std::unique_ptr<Model> m_model;
	Engine();

public:
	static Engine* getInstance() {
		if(instance == nullptr){
			instance = new Engine;
		}
		return instance;
	};

};




#endif //SOCKET_MODEL_HPP
