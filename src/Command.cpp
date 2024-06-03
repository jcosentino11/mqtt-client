#include "Command.h"
#include "Context.h"
#include "Packet.h"
#include <cstring>
#include <iostream>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace MqttClient {

Command::Command(Context context) : mContext(context) {}

void Command::execute() {
    if (mContext.verbose) {
        std::cout << "Executing " << mContext.command << " command\n";
    }

    auto packetBuilder = PacketBuilder(mContext);

    // TODO this needs major cleanup
    if (mContext.command == "pub") {

        Payload conn{};
        if (packetBuilder.connect(conn)) {
            // TODO move network stuff to separate class
            // TODO support ipv4
            // TODO resolve localhost, hosts

            int sock = socket(AF_INET6, SOCK_STREAM, 0);
            if (sock < 0) {
                std::cerr << "unable to create socket: " << strerror(errno)
                          << "\n";
                return; // TODO error handling
            }

            // TODO move this into context
            std::string host = mContext.address;
            std::string port = "1883";

            size_t pos = mContext.address.find_last_of(':');
            if (pos != std::string::npos) {
                host = mContext.address.substr(0, pos);
                port = mContext.address.substr(pos + 1);
            }
            std::cout << "host: " << host << "\n";
            std::cout << "port: " << port << "\n";

            sockaddr_in6 server_addr;
            std::memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin6_family = AF_INET6;
            server_addr.sin6_port = htons(std::stoi(port));
            if (inet_pton(AF_INET6, host.c_str(), &server_addr.sin6_addr) <=
                0) {
                close(sock);
                std::cerr << "unable to create address: " << strerror(errno)
                          << "\n";
                return;
            }

            if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) <
                0) {
                close(sock);
                std::cerr << "unable to connect to server: " << strerror(errno)
                          << "\n";
                return;
            }

            std::cout << "connected\n";

            if (send(sock, conn.data(), conn.size(), 0) < 0) {
                close(sock);
                std::cerr << "unable to send CONNECT: " << strerror(errno)
                          << "\n";
                return; // TODO error handling
            }

            std::cout << "CONNECT packet sent\n";

            std::cout << "waiting for resp\n";
            std::vector<char> buf(4);
            int bytes = recv(sock, buf.data(), buf.size(), 0);
            if (bytes == 4) {
                char packetType = buf[0];
                if (buf[0] == 0b00100000) {
                    int returnCode = buf[3];
                    bool sessionPresent = buf[2];
                    std::cout << "CONNACK received! return code:" << returnCode
                              << ", session present: " << sessionPresent
                              << "\n";
                    // TODO read session present
                }
                // TODO error handling
            } else {
                std::cout << "nothing received\n";
            }
        } else {
            std::cerr << "unable to build conn packet";
        }
    }
}

} // namespace MqttClient
