//
// Created by minkyung on 23. 3. 28.
//

#include "adapter.hpp"

using namespace std;

static boost::asio::io_context io_context;

bool Adapter::connect(short port) {
	unique_ptr<Server> s = unique_ptr<Server>(new Server(io_context, port));
	this->server = move(s);

	return true;
}

bool Adapter::run(){
	io_context.run();
}
