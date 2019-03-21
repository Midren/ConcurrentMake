#include <iostream>
#include <fcntl.h>
#include "libssh/libsshpp.hpp"
#include "string.h"

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "libssh/libsshpp.hpp"
#include "server_communnication.h"
#include "Node.h"


int main() {
    get_ips();
    return 0;
}