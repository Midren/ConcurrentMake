#ifndef CONCURRENT_MAKE_NODE_H
#define CONCURRENT_MAKE_NODE_H

#include <fcntl.h>
#include "libssh/libsshpp.hpp"
#include "string.h"
#include <iostream>

#include "Scp.h"

class Node {
public:
    explicit Node(std::string node_name);

    ~Node();

    void connect();

    std::string execute_command(std::string cmd);

    void scp_write_file(std::string filename, std::string text);

    std::string scp_read_file(std::string filename);

    void scp_copy_file_from_user(std::string from, std::string to);

    void scp_copy_file_from_server(std::string from, std::string to);


private:
    ssh::Session session;
    bool is_connected;
};


#endif //CONCURRENT_MAKE_NODE_H
