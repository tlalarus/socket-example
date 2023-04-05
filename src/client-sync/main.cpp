//
// Created by minkyung on 23. 3. 28.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>
#include <boost/asio.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>

#include "../../include/protocol.h"

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

		while(true){

			std::cout << "Select request type" << std::endl;
			std::cout << " 0:REQ_IS_STANDBY \n 1:REQ_IS_READY \n 2:REQ_IS_TRIGGERED \n 3:REQ_RESULT)" << std::endl;
			std::cout << "Enter req code:";

			int req_code = -1;
			std::cin >> req_code;

			// Serialize
			char request[max_length];
			request[0] = req_code;
			size_t req_len = std::strlen(request);
			std::cout << "1) Request from external client to adapter: req code= "  << req_code << ", req size= " << req_len << std::endl;
			std::string request_str = std::to_string(req_code);

			boost::asio::write(s, boost::asio::buffer(request, max_length));

			// Deserialize
			char reply[max_length];
			size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, req_len));
			std::string reply_str(reply, reply_length);
			std::cout << "2) Reply from adapter: reply size= " << reply_length << std::endl;
			std::cout.write(reply, reply_length);
			std::cout << "\n\n";
		}

	}
	catch(std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
