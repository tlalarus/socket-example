//
// Created by minkyung on 23. 3. 29.
//

#ifndef SOCKET_ASYNC_SERVER_HPP
#define SOCKET_ASYNC_SERVER_HPP

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <random>
#include <boost/asio.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>
#include "../include/protocol.h"

using boost::asio::ip::tcp;

// A reference-counted non-modifiable buffer class.
class shared_const_buffer{
public:
	//Construct from a std::string.
	explicit shared_const_buffer(const std::string& data)
					: data_(new std::vector<char>(data.begin(), data.end())),
					  buffer_(boost::asio::buffer(*data_)){

	}

	// Implement the ConstBufferSequence requirements.
	typedef boost::asio::const_buffer value_type;
	typedef const boost::asio::const_buffer* const_iterator;
	const boost::asio::const_buffer* begin() const { return &buffer_; }
	const boost::asio::const_buffer* end() const { return &buffer_ + 1; }

private:
	std::shared_ptr<std::vector<char>> data_;
	boost::asio::const_buffer buffer_;
};


// enable_shared_from_this 를 상속 받음으로써
// 공통된 raw pointer를 share 할 수 있게 해준다.(하나의 레퍼런스카운트로 관리됨.
class Session
				: public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, std::function<void(eRequestType)> req_handler)
					: socket_(std::move(socket)), bufs(b.prepare(1024)){
		request_handler_func = req_handler;
	}
	void start(){
		do_read();
	}

private:
	void do_read(){
		auto self(shared_from_this());
		socket_.async_read_some(boost::asio::buffer(data_, max_length),
		                        [this, self](boost::system::error_code ec, std::size_t length)
		                        {
				                        if(!ec){
																	// simple deserialize
																	std::string req_str(data_, length);
																	std::stringstream ss(req_str);
																	boost::archive::text_iarchive ia{ss};

					                        sRequestTest req_msg;
																	ia >> req_msg;

																	std::cout <<"3) Received request: code= " << req_msg.code << ", req size= " << length << std::endl;
																	// adapter callback func call
																	// response =
																	request_handler_func(static_cast<eRequestType>(req_msg.code));

																	// send response
					                        do_write(length);
				                        }
		                        });
	}
	void do_write(std::size_t length){

		sResponseTest res_msg = { 0, true, 2.22f, 3.33f };
		std::stringstream ss;
		boost::archive::text_oarchive oa{ss};
		oa << res_msg;

		ss << "\n";

		std::cout << "4) Send response: " << ss.str() << std::endl;
		shared_const_buffer res_buf(ss.str());

	// handle request
		auto self(shared_from_this());
		boost::asio::async_write(socket_, res_buf,
		                         [this, self](boost::system::error_code ec, std::size_t /*length*/)
		                         {
				                         if(!ec){
					                         do_read();
				                         }
		                         });
	}

	tcp::socket socket_;
	std::function<void(eRequestType)> request_handler_func;
	enum {max_length = (1024*5) };
	char data_[max_length*5];
	boost::asio::streambuf b;
	boost::asio::streambuf::const_buffers_type bufs;
};

class Server{
public:
	Server(boost::asio::io_context& io_context, short port,
				 std::function<void(eRequestType)> req_handler)
					: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)){
		request_handler_func = req_handler;
		do_accept();
	}

private:
	void do_accept(){
		acceptor_.async_accept(
						[this](boost::system::error_code ec, tcp::socket socket){
								if(!ec){
									std::cout << "2) Make shared_ptr of Session and pass request handler" << std::endl;
									std::make_shared<Session>(std::move(socket), request_handler_func)->start();
								}
								do_accept();
						});
	}
	tcp::acceptor acceptor_;
	std::function<void(eRequestType)> request_handler_func;
};

#endif //SOCKET_ASYNC_SERVER_HPP
