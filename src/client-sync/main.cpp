//
// Created by minkyung on 23. 3. 28.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
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

		int req_cnt = 0;
		int req_arr[3] = {0, 1, 3};

		while(true){

			std::cout << "Select request type" << std::endl;
			std::cout << " 0:REQ_IS_STANDBY \n 1:REQ_IS_READY \n 3:REQ_RESULT)" << std::endl;
			std::cout << "Enter to continue" << std::endl;
			getchar();

			if(req_cnt > 2){
				break;
			}
			int32_t req_code = req_arr[req_cnt++];
			std::cout << "Enter req code:" << req_code << std::endl;

			// Serialize
			sRequestTest req_msg = {req_code};
			std::stringstream ss;
			boost::archive::text_oarchive oa{ss};
			oa << req_msg;

			auto req_length = boost::asio::write(s, boost::asio::buffer(ss.str()));
			std::cout << "1) Request to adapter: " << ss.str() << std::endl;
//			std::cout << "1) Request to adapter: req code= "  << req_msg.code << ", req size= " << req_length << std::endl;
//			boost::asio::write(s, boost::asio::buffer(request_str));

			char res_arr[1024];
			boost::asio::streambuf buf;
			boost::system::error_code error;
			size_t reply_length = boost::asio::read_until(s, buf, '\n', error);
			std::string res_str(res_arr, reply_length);

			std::stringstream msg_ss;
			msg_ss.write(boost::asio::buffer_cast<const char*>(buf.data()), reply_length);
			boost::archive::text_iarchive ia{msg_ss};
			sResponseTest res_msg;
			ia >> res_msg;

			std::cout << "2) Reply from adapter: " << msg_ss.str() << std::endl;
			std::cout << "2) Reply from adapter: code= " << res_msg.code << ", values= " << res_msg.val1 << " " << res_msg.val2 << " " << res_msg.val3 << std::endl;

		}

	}
	catch(std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
