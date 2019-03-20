#include <iostream>
#include <fcntl.h>
#include "string.h"

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "libssh/libsshpp.hpp"

#include "Node.h"

namespace http = boost::beast::http;

int main() {
//    Node n("YuraYeliseev@10.10.226.136");
//    n.connect();
//
//    std::cout << n.execute_command("ls");
//    n.scp_write_file("hello.txt", "No Problema, Houston\n");
//    std::cout << n.scp_read_file("hello.txt");
    const std::string host = "";
    const std::string target = "/";

    // I/O контекст, необходимый для всех I/O операций
    boost::asio::io_context ioc;

    // Resolver для определения endpoint'ов
    boost::asio::ip::tcp::resolver resolver(ioc);
    // Tcp сокет, использующейся для соединения
    boost::asio::ip::tcp::socket socket(ioc);

    // Резолвим адрес и устанавливаем соединение
    boost::asio::connect(socket, resolver.resolve(host, "80"));

    // Дальше необходимо создать HTTP GET реквест с указанием таргета
    http::request <http::string_body> req(http::verb::get, target, 11);
    // Задаём поля HTTP заголовка
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Отправляем реквест через приконекченный сокет
    http::write(socket, req);

    // Часть, отвечающая за чтение респонса
    {
        boost::beast::flat_buffer buffer;
        http::response <http::dynamic_body> res;
        http::read(socket, buffer, res);

        std::cout << res << std::endl;
    }
    // Закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    return 0;
}