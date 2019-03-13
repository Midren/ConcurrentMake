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
    scp_write(session);
    return 0;
}