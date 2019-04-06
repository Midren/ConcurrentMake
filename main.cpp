#include <iostream>
#include <filesystem>

#include "server_communnication.h"
#include "Node.h"

namespace fs = std::filesystem;

void cp_files() {
    update_ips_json();
    auto ips = get_ips();
    for (auto x: ips)
        std::cout << x << " ";
    Node n(ips[0]);
    n.connect();

    fs::path working_directory("/home/midren/Documents/tmp/file.txt");
    for (auto &p: fs::directory_iterator("../"))
        if (!p.is_directory())
            n.scp_send_file(p.path(), working_directory.replace_filename(p.path().filename()));
}

int main() {
    return 0;
}