#include <iostream>
#include <fcntl.h>
#include "libssh/libsshpp.hpp"
#include "string.h"

#include "Node.h"


int main() {
    Node n("YuraYeliseev@10.10.226.136");
    n.connect();

    std::cout << n.execute_command("ls");
    n.scp_write_file("hello.txt", "No Problema, Houston\n");
    std::cout << n.scp_read_file("hello.txt");
    return 0;
}