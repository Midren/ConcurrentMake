#include <iostream>
#include <fcntl.h>
#include <sstream>
#include "libssh/libsshpp.hpp"
#include "string.h"

void scp_write_file(ssh::Session &session, std::string filename, std::string text) {
    ssh_scp scp;
    scp = ssh_scp_new(session.getCSession(), SSH_SCP_WRITE, ".");
    if (scp == nullptr) {
        std::cout << "Error allocationg scp session\n";
        throw ssh::SshException(session.getCSession());
    }
    if (ssh_scp_init(scp) != SSH_OK) {
        std::cout << "Eror initializing scp session\n";
        ssh_scp_free(scp);
        throw ssh::SshException(session.getCSession());
    } else {
        if (ssh_scp_push_file(scp, filename.c_str(), text.length(), S_IRUSR | S_IWUSR) != SSH_OK) {
            std::cout << "Cannot push file" << std::endl;
            throw ssh::SshException(session.getCSession());
        }
        if (ssh_scp_write(scp, text.c_str(), text.length()) != SSH_OK) {
            std::cout << "Cannot write to file" << std::endl;
            throw ssh::SshException(session.getCSession());
        }
        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }
}

std::string scp_read_file(ssh::Session &session, std::string filename) {
    ssh_scp scp;
    scp = ssh_scp_new(session.getCSession(), SSH_SCP_READ, filename.c_str());
    if (scp == nullptr) {
        std::cout << "Error allocationg scp session\n";
        throw ssh::SshException(session.getCSession());
    }
    if (ssh_scp_init(scp) != SSH_OK) {
        std::cout << "Eror initializing scp session\n";
        ssh_scp_free(scp);
        throw ssh::SshException(session.getCSession());
    }
    if (ssh_scp_pull_request(scp) != SSH_SCP_REQUEST_NEWFILE) {
        std::cout << "Erro getting info about file" << std::endl;
        throw ssh::SshException(session.getCSession());
    }
    size_t size = ssh_scp_request_get_size(scp);
    char *buffer = new char[size];
    ssh_scp_accept_request(scp);
    ssh_scp_read(scp, buffer, size);

    ssh_scp_close(scp);
    ssh_scp_free(scp);
    return std::string(buffer);
}

void execute_command(ssh::Session &session, std::string cmd) {
    ssh::Channel channel(session);
    channel.openSession();
    ssh_channel_request_pty(channel.getCChannel());
    channel.requestExec(cmd.c_str());
    int nbytes;
    char buffer[256];
    nbytes = ssh_channel_read(channel.getCChannel(), buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        if (write(1, buffer, nbytes) != (unsigned int) nbytes) {
            throw ssh::SshException(session.getCSession());
        }
        nbytes = ssh_channel_read(channel.getCChannel(), buffer, sizeof(buffer), 0);
    }
}

int main() {
    ssh::Session session;
    session.setOption(SSH_OPTIONS_HOST, "midren@localhost");
    try {
        session.connect();
    } catch (ssh::SshException &e) {
        std::cout << e.getError();
    }
    char *password = getpass("Password: ");
    if (ssh_userauth_password(session.getCSession(), nullptr, password) != SSH_AUTH_SUCCESS) {
        std::cout << "Wrong password";
    }
//    scp_write_file(session, "hello.txt", "Yeah, it' works\nMaybe we will end this\n");
    execute_command(session, "echo Super > hello.txt");
    std::cout << scp_read_file(session, "hello.txt");
    return 0;
}