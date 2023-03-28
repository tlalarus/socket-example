#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

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
																do_write(length);
															}
														});
	}
	void do_write(std::size_t length){
		auto self(shared_from_this());
		boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
														 [this, self](boost::system::error_code ec, std::size_t /*length*/)
														 {
																if(!ec){
																	do_read();

																	std::cout << "request from client: " << data_ << std::endl;
																}
														 });
	}

	tcp::socket socket_;
	enum {max_length = 1024 };
	char data_[max_length];
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

int main(int argc, char* argv[]){
	try{
		if(argc != 2){
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
		}

		boost::asio::io_context io_context;

		Server s(io_context, std::atoi(argv[1]));
		io_context.run();
	}
	catch(std::exception& e){
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}