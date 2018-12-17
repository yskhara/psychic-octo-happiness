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
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

static constexpr uint PORT_NUM = 31400;

int main()
{
    int    i;
    int    fd1, fd2;
    struct sockaddr_in    saddr;
    struct sockaddr_in    caddr;
    int    len;
    int    ret;
    char   buf[1024];




    asio::io_service io_service;

    tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), PORT_NUM));
    tcp::socket socket(io_service);

    boost::system::error_code error;
    acc.accept(socket, error);

    if (error) {
        std::cout << "accept failed: " << error.message() << std::endl;
    }
    else {
        std::cout << "accept correct!" << std::endl;
    }



    while(1)
    {
    	// receive data
		asio::streambuf receive_buffer;
		asio::read(socket, receive_buffer, asio::transfer_all(), error);

		if (error && error != asio::error::eof) {
			std::cout << "receive failed: " << error.message() << std::endl;

			continue;
		}



		const std::string msg = asio::buffer_cast<const char*>(receive_buffer.data());
		std::cout << msg << std::endl;



    	// echo back
		//const std::string msg = "ping";
		asio::write(socket, asio::buffer(msg), error);

		if (error) {
			std::cout << "send failed: " << error.message() << std::endl;
		}
		else {
			std::cout << "send correct!" << std::endl;
		}


    }

    /* クライアントと接続されているソケットからデータを受け取る */

    do{
      ret = read(fd2, buf, 1024);

      /* bufの中の小文字を大文字に変換する */
      for (i=0; i< ret; i++) {
	if (isalpha(buf[i]))
	  buf[i] = toupper(buf[i]);
      }

      /* 変換したデータをクライアントに送り返す */
      write(fd2, buf, 1024);
    }
    while (strcmp(buf, "QUIT\n") != 0);

    /* 通信が終わったらソケットを閉じる */
    close(fd2);
}
