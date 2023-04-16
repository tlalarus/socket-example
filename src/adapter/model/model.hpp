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
#include <chrono>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../include/protocol.h"


//#include <functional>
//using CallbackFn = std::function<void(eResponseType code, struct _payload payloads)>;

// Server
class Message{
public:
	Message() {};
	~Message(){};

	// response가 준비 되었을때 Adapter 에게 notify할 콜백함수 등록하는 함수
	static void registerNotiCB(NotiCallbackFn pfunc) {
		func_response = pfunc;
//		func_response = std::bind(pfunc, std::placeholders::_1, std::placeholders::_2);
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
	void sendResponse(sResponseTest response_){
//		payload pl;
//		std::size_t msgsize = noti_msg.size();
//
//		pl.val = new uint8_t[msgsize];
//		pl.length = msgsize;
//
//		memcpy(pl.val, noti_msg.c_str(), msgsize);

		// send
		func_response(response_);

//		delete pl.val;
	};

private:
	static NotiCallbackFn func_response;
	static sRequest req_msg;

};

class Engine;

class Model{
public:
	Model() = default;
	~Model() = default;

	void do_something1(Engine& engine_, int action_code) {
		std::cout << "do_something1() act: " << action_code << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	};
	void do_something2(Engine& engine_, int action_code) {
		std::cout << "do_something2() act: " << action_code << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
	};
	void notifyError(Engine& engine_);
};

// TODO
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
	std::shared_ptr<Model> m_model;
	Engine();

	boost::asio::thread_pool pool;
public:
	static Engine* getInstance() {
		if(instance == nullptr){
			instance = new Engine;
		}
		return instance;
	};
	void postTask(std::function<void(Engine&, int)>& task, int code){
		boost::asio::post(pool, [&](){ task(*instance, code); });

		// 최초 post 를 의미하는 코드가 입력되면 pool join 해주기
		if(code == 0){
			std::cout << "7) Run thread pool" << std::endl;
			pool.join(); // 리턴값이 필요하다면 future형 반환 만들기
		}
	}

};




#endif //SOCKET_MODEL_HPP
