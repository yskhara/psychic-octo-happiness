/*
 * server.c
 *   クライアントからの接続要求を受け付けるサーバープログラム。
 *
 *   クライアントから送られてきた文字列を大文字に変換して送り返す。
 */

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/types.h>   /* socket() */
//#include <sys/socket.h>  /* socket() */
//#include <netinet/in.h>  /* #include <sys/un.h>の代わり */
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

static constexpr uint PORT_NUM = 31400;

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

void async_receive(boost::system::error_code const& error,
		size_t bytes_transferred) {
	if ((boost::asio::error::eof == error)
			|| (boost::asio::error::connection_reset == error)) {
		// disconnected

	} else {
		// read the data

	}
}

int main() {
	int i;
	int fd1, fd2;
	struct sockaddr_in saddr;
	struct sockaddr_in caddr;
	int len;
	int ret;
	char buf[1024];

	asio::io_service io_service;

	tcp::socket socket(io_service);
	tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), PORT_NUM));

	boost::system::error_code error;


	  boost::asio::io_context io;
	  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));

	while (1) {
		//socket.open(tcp::v4());
		acc.accept(socket, error);

		if (error) {
			std::cout << "accept failed: " << error.message() << std::endl;
		} else {
			std::cout << "accept correct!" << std::endl;
		}

		// receive data
		asio::streambuf receive_buffer;
		asio::read(socket, receive_buffer, asio::transfer_at_least(1), error);

		if (error && error != asio::error::eof) {
			std::cout << "receive failed: " << error.message() << std::endl;

			continue;
		}

		std::stringstream msg(asio::buffer_cast<const char*>(receive_buffer.data()));


		std::string buf;
		std::vector<std::string> v;
		while (std::getline(msg, buf, ',')) {
		  v.push_back(buf);
		}

		// print received data
		std::cout << "ip: " << v[0] << std::endl;
		std::cout << "port: " << v[1] << std::endl;
		std::cout << "id: " << v[2] << std::endl;

		//const std::string msg = "ping";
		asio::write(socket, asio::buffer("ACK"), error);

		/*
		// echo back
		//const std::string msg = "ping";
		asio::write(socket, asio::buffer(msg.str()), error);

		if (error) {
			std::cout << "send failed: " << error.message() << std::endl;
		} else {
			std::cout << "send correct!" << std::endl;
		}*/

		//acc.close();
		socket.close();
	}

	/* クライアントと接続されているソケットからデータを受け取る */

	do {
		ret = read(fd2, buf, 1024);

		/* bufの中の小文字を大文字に変換する */
		for (i = 0; i < ret; i++) {
			if (isalpha(buf[i]))
				buf[i] = toupper(buf[i]);
		}

		/* 変換したデータをクライアントに送り返す */
		write(fd2, buf, 1024);
	} while (strcmp(buf, "QUIT\n") != 0);

	/* 通信が終わったらソケットを閉じる */
	close(fd2);
}
