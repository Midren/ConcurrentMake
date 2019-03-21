#include <iostream>
#include <fcntl.h>
#include "libssh/libsshpp.hpp"
#include "string.h"
#include <filesystem>

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "libssh/libsshpp.hpp"
#include "server_communnication.h"
#include "Node.h"

namespace fs = std::filesystem;

int main() {
    Node n("midren@localhost");
    n.connect();

    fs::path working_directory("/home/midren/Documents/tmp/file.txt");
    for (auto &p: fs::directory_iterator("../"))
        if (!p.is_directory())
            n.scp_send_file(p.path(), working_directory.replace_filename(p.path().filename()));
    return 0;
}