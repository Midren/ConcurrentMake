#include <iostream>
#include <fcntl.h>
#include "libssh/libsshpp.hpp"
#include "string.h"

int scp_write(ssh::Session &session, std::string filename, std::string text) {
    ssh_scp scp;
    scp = ssh_scp_new(session.getCSession(), SSH_SCP_WRITE, ".");
    if (scp == nullptr) {
        std::cout << "Error allocationg scp session\n";
    }
    if (ssh_scp_init(scp) != SSH_OK) {
        std::cout << "Eror initializing scp session\n";
        ssh_scp_free(scp);
    } else {
        if (ssh_scp_push_file(scp, filename.c_str(), text.length(), S_IRUSR | S_IWUSR) != SSH_OK) {
            std::cout << "Cannot push file" << std::endl;
        }
        if (ssh_scp_write(scp, text.c_str(), text.length()) != SSH_OK) {
            std::cout << "Cannot write to file" << std::endl;
        }
        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }
    return SSH_OK;
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
    if (ssh_userauth_password(session.getCSession(), NULL, password) != SSH_AUTH_SUCCESS) {
        std::cout << "Wrong password";
    }
    scp_write(session, "hello.txt", "Hello, world!\n");
    return 0;
}