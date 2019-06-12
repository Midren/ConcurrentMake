#include <iostream>
#include <filesystem>

#include "server_communication.h"
#include "dependecies_parser.h"
#include "SSH_Node/Node.h"
#include "ccmake.h"
#include "node_daemon/sys_spec.h"

namespace fs = std::filesystem;

void cp_headers(Node &n, fs::path working_directory, fs::path dst) {
    for (auto &p: fs::directory_iterator(working_directory))
        if (!p.is_directory()) {
            if (p.path().extension() == ".h") {
                n.scp_send_file(p.path(), dst.replace_filename(p.path().filename()));
            }
        }
}

void cp_cpp(Node &n, fs::path working_directory, fs::path dst) {
    for (auto &p: fs::directory_iterator(working_directory))
        if (!p.is_directory()) {
            if (p.path().extension() == ".cpp" || p.path().extension() == ".c") {
                n.scp_send_file(p.path(), dst.replace_filename(p.path().filename()));
            }
        }
}

void cp_files(Node &n, std::vector<std::string> files, fs::path working_directory, fs::path dst) {
    for (auto &file: files) {
        fs::path p = working_directory / file;
        n.scp_send_file(p, dst.replace_filename(p.filename()));
    }
}

void show_dependency(fs::path dir) {
    list_project_dependencies(dir.string());
    auto a = create_graph("../dep");
    print_graph(a);
    system("dot -Tpng ../dep/dep_graph.dot > dependency_graph.png");
}

void link_whole_project(fs::path ccmake_directory, fs::path project_directory) {
    system(std::string("bash cd " + project_directory.parent_path().string() + " && " +
                       ccmake_directory.append("scripts/link_whole.sh").string()).c_str());
}

int main(int argc, char **argv) {
    fs::path cur_directory(fs::current_path().parent_path());
    fs::path project_directory(static_cast<std::string>(argv[1]));
    auto args = getCcmakeArgs(project_directory.string());
    project_directory = project_directory.parent_path();

    auto os_name = get_os();
    auto[compiler, major, minor] = compiler_version();
    std::map<std::string, std::string>
            params = {{"compiler_name", compiler},
                      {"major_version", std::to_string(major)},
                      {"minor_version", std::to_string(minor)},
                      {"os_name",       os_name}};

    update_ips_json("/get_ips", params);
    auto ips = get_ips();

    std::vector<std::string> sources, headers;
    boost::split(headers, args->headers, boost::is_any_of(","));
    boost::split(sources, args->sources, boost::is_any_of(","));

    Node n(ips[0]);
    n.connect();

    //  Create library with working files
    n.execute_command("rm -rf ~/.project && mkdir .project", false);
    cp_files(n, sources, project_directory, fs::path("~/.project/"));
    cp_files(n, headers, project_directory, fs::path("~/.project/"));


    // Make static library in remote computer
    n.scp_send_file("../scripts/generate_lib.sh", fs::path("~/.project/generate_lib.sh"));
    n.execute_command("chmod +x ~/.project/generate_lib.sh", false);
    n.execute_command("cd ~/.project && ./generate_lib.sh && echo 'a' ", true);

    // Create folder for static libraries
    fs::current_path(project_directory);
    fs::create_directories(project_directory / "libs");
    project_directory.append("libs/");

    // Get static library from remote computer
    n.scp_download_file("~/.project/libs/lib1.a", project_directory.replace_filename("lib1.a"));
//    }

    link_whole_project(cur_directory, project_directory);
    return 0;
}