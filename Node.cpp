#include <fstream>
#include "Node.h"

Node::Node(std::string node_name) : session(), is_connected(false) {
    session.setOption(SSH_OPTIONS_HOST, node_name.c_str());
};

Node::~Node() {
    if (is_connected) {
        session.disconnect();
    }
}

void Node::connect() {
    session.connect();
    is_connected = true;
    if (session.userauthPublickeyAuto() != SSH_AUTH_SUCCESS) {
        throw std::runtime_error("Didn't auth");
    }
}


std::string Node::execute_command(std::string cmd) {
    ssh::Channel channel(session);
    channel.openSession();
    ssh_channel_request_pty(channel.getCChannel());
    channel.requestExec(cmd.c_str());
    int nbytes;
    char buffer[256];
    std::string ret;
    nbytes = channel.read(buffer, sizeof(buffer), false, -1);
    while (nbytes > 0) {
        ret.append(buffer);
        nbytes = ssh_channel_read(channel.getCChannel(), buffer, sizeof(buffer), false);
    }
    return ret;
}

void Node::scp_write_file(std::filesystem::path path_to_file, std::string text) {
    Scp scp(session, SSH_SCP_WRITE, path_to_file.parent_path());
    scp.push_file(path_to_file.filename().c_str(), text.length(), S_IRUSR | S_IWUSR);
    scp.write(text);
}

std::string Node::scp_read_file(std::filesystem::path path_to_file) {
    Scp scp(session, SSH_SCP_READ, path_to_file.c_str());

    if (scp.pull_request() != SSH_SCP_REQUEST_NEWFILE) {
        throw std::runtime_error("Invalid request from scp");
    }

    scp.accept_request();
    std::string file = scp.read();

    return file;
}

void Node::scp_send_file(std::filesystem::path from, std::filesystem::path to) {
    std::cout << to << std::endl;
    std::ifstream input(from, std::ios::binary);
    if (input.is_open()) {
        size_t buf_size = std::filesystem::file_size(from);
        char *buf = new char[buf_size];
        input.read(buf, buf_size);

        std::string output(buf);
        free(buf);
        Node::scp_write_file(to, output);
    }
    input.close();
};

void Node::scp_download_file(std::filesystem::path from, std::filesystem::path to) {
    std::string input = scp_read_file(from);
    std::ofstream(to, std::ios::binary).write(input.c_str(), input.length());
}


