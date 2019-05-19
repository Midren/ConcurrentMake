#include <iostream>
#include <filesystem>

#include "server_communnication.h"
#include "dependecies_parser.h"
#include "Node.h"

namespace fs = std::filesystem;

void cp_headers(Node &n, fs::path working_directory, fs::path dst) {
    for (auto &p: fs::directory_iterator(working_directory))
        if (!p.is_directory()) {
            if (p.path().extension() == ".h") {
//                n.scp_send_file(p.path(), working_directory.replace_filename(p.path().filename()));
                std::cout << p.path().string() << " " << dst.replace_filename(p.path().filename()).string()
                          << std::endl;
                n.scp_send_file(p.path(), dst.replace_filename(p.path().filename()));
            }
        }
}

void cp_cpp(Node &n, fs::path working_directory, fs::path dst) {
    for (auto &p: fs::directory_iterator(working_directory))
        if (!p.is_directory()) {
            if (p.path().extension() == ".cpp") {
//                n.scp_send_file(p.path(), working_directory.replace_filename(p.path().filename()));
                std::cout << p.path().string() << " " << dst.replace_filename(p.path().filename()).string()
                          << std::endl;
                n.scp_send_file(p.path(), dst.replace_filename(p.path().filename()));
            }
        }
}

int main() {
//    system("pwd");
////    system("bash ../scripts/test_script.sh");
//    std::cout << system("whoami");

//    update_ips_json();
//    auto ips = get_ips();
//    for (auto x: ips)
//        std::cout << x << " ";
//    std::cout << std::endl;
//    Node n(ips[0]);
//    n.connect();
//
    fs::path working_directory("/home/midren/Documents/ucu/example_distributed");
//    n.execute_command("rm -rf ~/.project && mkdir .project", false);
//    cp_headers(n, working_directory, fs::path("~/.project/"));
//    n.scp_send_file("../scripts/generate_lib.sh", fs::path("~/.project/generate_lib.sh"));
//    n.scp_send_file("../scripts/link_whole.sh", fs::path("~/.project/link_whole.sh"));
//    n.execute_command("chmod +x ~/.project/link_whole.sh", false);
//    n.execute_command("chmod +x ~/.project/generate_lib.sh", false);
//    n.execute_command("cd ~/.project && ./generate_lib.sh", false);
    list_project_dependencies(working_directory.string());
    auto a = create_graph("../dep");
    print_graph(a);
    system("dot -Tpng ../dep/dep_graph.dot > output.png");
    return 0;
}