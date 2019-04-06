#include "server_communnication.h"


void update_ips_json() {
    std::string target = "/get_ips";
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    boost::asio::connect(socket, resolver.resolve(website, "80"));
    http::request<http::string_body> req(http::verb::get, target, 11);
    req.set(http::field::host, website);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);


    boost::beast::flat_buffer buffer;
    http::response<http::string_body > res;
    http::read(socket, buffer, res);
    std::string res_body = res.body();
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    std::ofstream json_file("addresses_ip.json");
    json_file << res_body;
    json_file.close();
}

std::vector<std::string> get_ips() {
    boost::property_tree::ptree pt;
    boost::property_tree::read_json("addresses_ip.json", pt);

    pt = pt.get_child("addresses");
    std::vector<std::string> addresses;
    for(boost::property_tree::ptree::iterator iter = pt.begin(); iter != pt.end(); iter++)
    {
       for (auto &i: iter->second){
           addresses.push_back(i.second.data());
       }
    }
    return addresses;
}
