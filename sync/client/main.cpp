//
// Created by minkyung on 23. 3. 28.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

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

		std::cout << "Enter message: ";
		char request[max_length];
		std::cin.getline(request, max_length);
		size_t request_length = std::strlen(request);

		boost::asio::write(s, boost::asio::buffer(request, request_length));

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
