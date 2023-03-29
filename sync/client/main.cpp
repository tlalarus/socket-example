//
// Created by minkyung on 23. 3. 28.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>

#include "../../adapter/protocol.h"

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[]){
	try{
		if(argc != 3){
			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
			return 1;
		}

		// 어플과 커널 사이에서 중간다리 역할을 해주는 것임.
		boost::asio::io_context io_context;
		tcp::socket s(io_context);

		// resolver는 연결하고자하는 서버의 주소에 대해 DNS query를 해준다.
		// 즉, 어플에서 DNS 이름을 확인해주는 인터페이스를 제공해준다.(실제 연결가능한 주소인지)
		tcp::resolver resolver(io_context);
		boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));

		std::cout << "Enter to request: ";
		char request[max_length*5];
//		std::cin.getline(request, max_length);

		// serialize
		boost::iostreams::stream<boost::iostreams::array_sink> os(request);
		boost::archive::binary_oarchive oa(os);
		std::map<int, std::string> req;
		req.insert({1, "ready"});
		oa << req;

		size_t request_length = std::strlen(request);
		std::cout << "request data size: " << request_length << std::endl;

		boost::asio::write(s, boost::asio::buffer(request, (max_length*5)));
//		boost::asio::write(s, buf);

		char reply[max_length];
		size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request_length));
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";
	}
	catch(std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
