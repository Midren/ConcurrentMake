#include <iostream>
#include "libssh/libsshpp.hpp"

int scp_write(ssh::Session &session) {
    ssh_scp scp;
    int rc;
    scp = ssh_scp_new(session.getCSession(), SSH_SCP_WRITE, ".");
    if (scp == NULL) {
        std::cout << "Error allocationg scp session\n";
    }
    rc = ssh_scp_init(scp);
    if (rc != SSH_OK) {
        std::cout << "Eror initializing scp session\n";
        ssh_scp_free(scp);
    } else {

        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }
    return SSH_OK;
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

    ssh::Channel channel(session);
    channel.openSession();
    ssh_channel_request_pty(channel.getCChannel());
    channel.requestExec("ls");
    int nbytes;
    char buffer[256];
    nbytes = ssh_channel_read(channel.getCChannel(), buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        if (write(1, buffer, nbytes) != (unsigned int) nbytes) {
            return SSH_ERROR;
        }
        nbytes = ssh_channel_read(channel.getCChannel(), buffer, sizeof(buffer), 0);
    }
    return 0;
}