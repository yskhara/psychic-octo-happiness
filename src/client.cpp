/*
 * client.c
 *    ソケットを使用して、サーバーに接続するクライアントプログラム。
 *
 *    入力された文字列をサーバーに送り、サーバーが大文字に変換したデータを
 *    受け取る。
 *
 */

#include <iostream>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using asio::ip::tcp;

static constexpr uint PORT_NUM = 31400;

int main(int argc, char *argv[])
{
    struct sockaddr_in    addr;
    struct hostent *hp;
    int    fd;
    int    len;
    char   buf[1024];
    int    ret;

    if (argc != 2){
		printf("Usage: iclient SERVER_NAME\n");
		exit(1);
    }

    std::vector<std::string> allArgs(argv + 1, argv + argc);

    asio::io_service io_service;
    tcp::socket socket(io_service);

    boost::system::error_code error;
    socket.connect(tcp::endpoint(asio::ip::address::from_string(allArgs[0]), PORT_NUM), error);

    if (error) {
        std::cout << "connect failed : " << error.message() << std::endl;
    }
    else {
        std::cout << "connected" << std::endl;
    }

    /*
     *  入力されたデータをソケットに書き込んでサーバーに送り、
     *  サーバーが変換して送ってきたデータを読み込む。
     */
    while (fgets(buf, 1024, stdin)) {

        // メッセージ送信
        const std::string msg = "ping";
        boost::system::error_code error;
        asio::write(socket, asio::buffer(msg), error);

        if (error) {
            std::cout << "send failed: " << error.message() << std::endl;
        }
        else {
            std::cout << "send correct!" << std::endl;
        }
    }

    exit(0);
}
