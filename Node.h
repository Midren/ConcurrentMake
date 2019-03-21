#ifndef CONCURRENT_MAKE_NODE_H
#define CONCURRENT_MAKE_NODE_H

#include <fcntl.h>
#include "libssh/libsshpp.hpp"
#include "string.h"
#include <iostream>
#include <filesystem>

#include "Scp.h"

class Node {
public:
    explicit Node(std::string node_name);

    ~Node();

    void connect();

    std::string execute_command(std::string cmd);

    void scp_write_file(std::filesystem::path path_to_file, std::string text);

    std::string scp_read_file(std::filesystem::path path_to_file);

    void scp_send_file(std::filesystem::path from, std::filesystem::path to);

    void scp_download_file(std::filesystem::path from, std::filesystem::path to);


private:
    ssh::Session session;
    bool is_connected;
};


#endif //CONCURRENT_MAKE_NODE_H
