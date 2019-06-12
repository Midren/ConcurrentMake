#include "requests.h"


std::string get(std::string target) {
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    boost::asio::connect(socket, resolver.resolve(website, "80"));
    http::request<http::string_body> req(http::verb::get, target, 11);
    req.set(http::field::host, website);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(socket, req);


    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(socket, buffer, res);
    std::string res_body = res.body();
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    return res_body;
}

std::vector<std::string> get_public_keys(std::string &input_json) {
    boost::property_tree::ptree pt;
    std::istringstream is(input_json);
    boost::property_tree::read_json(is, pt);

    pt = pt.get_child("public_keys");
    std::vector<std::string> addresses;
    for (boost::property_tree::ptree::iterator iter = pt.begin(); iter != pt.end(); iter++) {
        for (auto &i: iter->second) {
            addresses.push_back(i.second.data());
        }
    }
    return addresses;
}


std::string
put_ip(std::string &login, std::string &ip, std::string &public_key, std::string &linux_dist, std::string &compiler,
       int major, int minor) {
    boost::property_tree::ptree root;
    root.put("login", login);
    root.put("ip", ip);
    root.put("public_key", public_key);
    root.put("os_name", linux_dist);
    root.put("compiler_name", compiler);
    root.put("major_version", major);
    root.put("minor_version", minor);


    std::ostringstream buf;
    boost::property_tree::write_json(buf, root, false);
    std::string json = buf.str();
    std::cout << json << std::endl;
    std::string target = "/nodes/3";
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::asio::ip::tcp::socket socket(ioc);

    boost::asio::connect(socket, resolver.resolve(website, "80"));
    http::request<http::string_body> req(http::verb::put, target, 11);
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
