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
#include <boost/serialization/serialization.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>
#include "protocol.h"

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
	Session(tcp::socket socket)
					: socket_(std::move(socket)){

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
																	// deserialize
																	boost::iostreams::stream<boost::iostreams::array_source> is(data_);
																	boost::archive::binary_iarchive ia(is);

																	std::map<int, std::string> request;
																	ia >> request;

																	for(auto& m : request){
																		std::cout << "req code: " << m.first;
																		std::cout << " value: " << m.second << std::endl;
																	}
																	// adapter callback func call
//																	request_handler_func

																	// send response
					                        do_write(length);
				                        }
		                        });
	}
	void do_write(std::size_t length){
		std::mt19937_64 rng1(1029);
		std::uniform_int_distribution<int64_t> dist1(0,100);

		std::cout << "dist1 Min : " << dist1.min() << std::endl;
		std::cout << "dist1 Max : " << dist1.max() << std::endl;
		int response = dist1(rng1);
		std::cout << "response : " << response << std::endl;

		shared_const_buffer buffer(std::to_string(response));

		auto self(shared_from_this());
		boost::asio::async_write(socket_, buffer,
		                         [this, self](boost::system::error_code ec, std::size_t /*length*/)
		                         {
				                         if(!ec){
					                         do_read();
				                         }
		                         });
	}

	tcp::socket socket_;
	std::function<void(eResponseType code, payload& pl)> request_handler_func;
	enum {max_length = (1024*5) };
	char data_[max_length*5];
	boost::asio::streambuf stream_buf;
};

class Server{
public:
	Server(boost::asio::io_context& io_context, short port)
					: acceptor_(io_context, tcp::endpoint(tcp::v4(), port)){
		do_accept();
	}

private:
	void do_accept(){
		acceptor_.async_accept(
						[this](boost::system::error_code ec, tcp::socket socket){
								if(!ec){
									std::make_shared<Session>(std::move(socket))->start();
								}
								do_accept();
						});
	}
	tcp::acceptor acceptor_;
};

#endif //SOCKET_ASYNC_SERVER_HPP
