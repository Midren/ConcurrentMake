//
// Created by danylo.kolinko on 5/19/19.
//

#include "background_commun.h"
#include <iostream>
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


std::pair<std::string, std::string> get_public_key_and_login() {
    std::ifstream public_key_file{std::string(getenv("HOME")) + "/.ssh/id_rsa.pub"};
    std::string current, public_key, login;
    int iter = 0;
    while (std::getline(public_key_file, current, ' ')) {
        switch (iter) {
            case 1: {
                public_key = current;
                break;
            }
            case 2: {
                login = current.substr(0, current.find('@'));
                break;
            }
            default:
                break;

        }
        ++iter;
    }
    return make_pair(public_key, login);
}

std::string post_ip(std::string &login, std::string &ip, std::string &public_key) {
    boost::property_tree::ptree root;
    root.put("login", login);
    root.put("ip", ip);
    root.put("public_key", public_key);


    std::ostringstream buf;
    boost::property_tree::write_json(buf, root, false);
    std::string json = buf.str();

    std::string target = "/";
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    boost::asio::connect(socket, resolver.resolve(website, "80"));
    http::request<http::string_body> req(http::verb::post, target, 11);
    req.set(http::field::host, website);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/json");
    req.keep_alive(req.keep_alive());
    req.body() = json;
    req.prepare_payload();
    http::write(socket, req);


    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(socket, buffer, res);
    std::string res_body = res.body();
    boost::system::error_code ec;
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

    return res_body;
}

void get_local_ip() {
    //TODO: rewrite get local ip part using pure c++.
    while(true) {
        const char *google_dns_server = "8.8.8.8";
        int dns_port = 53;

        struct sockaddr_in serv;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        //Socket could not be created
        if (sock < 0) {
            std::cout << "Socket error" << std::endl;
        }

        memset(&serv, 0, sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = inet_addr(google_dns_server);
        serv.sin_port = htons(dns_port);

        int err = connect(sock, (const struct sockaddr *) &serv, sizeof(serv));
        if (err < 0) {
            std::this_thread::sleep_for(std::chrono::hours(4));
            continue;
        }

        struct sockaddr_in name;
        socklen_t namelen = sizeof(name);
        err = getsockname(sock, (struct sockaddr *) &name, &namelen);

        char buffer[80];
        const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
        if (p != nullptr) {
            std::string ip{buffer};
            auto params = get_public_key_and_login();
            std::string public_key = params.first;
            std::string login = params.second;
            post_ip(login, ip, public_key);
            std::this_thread::sleep_for(std::chrono::hours(1));
        } else {
            std::cout << "Error number: " << errno
                      << ". Error message: " << strerror(errno) << std::endl;
        }

        close(sock);
    }
}

int main(int argc, char *argv[]) {



    pid_t pid, sid;

    //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //----------------
    //Main Process
    //----------------
    get_local_ip();




}