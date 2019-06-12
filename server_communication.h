#ifndef CONCURRENT_MAKE_SERVER_COMMUNNICATION_H
#define CONCURRENT_MAKE_SERVER_COMMUNNICATION_H

#include <map>

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace http = boost::beast::http;
//static const std::string website = "yeliseev.pythonanywhere.com";


void update_ips_json(std::string target, std::map<std::string, std::string>);

std::vector<std::string> get_ips();


#endif //CONCURRENT_MAKE_SERVER_COMMUNNICATION_H
