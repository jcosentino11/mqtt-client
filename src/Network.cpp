#include "Network.h"
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
Network::Network(std::shared_ptr<Context> context)
    : mContext(std::move(context)) {};
Network::~Network() { netClose(); }

bool Network::netSend(Payload &payload) {
    if (!netConnect()) {
        return false;
    }
    if (send(mSock, payload.data(), payload.size(), 0) < 0) {
        netClose();
        return false;
    }
    return true;
}

bool Network::netRecv(Payload &payload, size_t len) {
    if (!netConnect()) {
        return false;
    }

    payload.reserve(len);

    auto totalReceived = 0;
    while (totalReceived < len) {
        auto received =
            recv(mSock, payload.data() + totalReceived, len - totalReceived, 0);
        if (received == 0) {
            netClose();
            return false;
        }
        totalReceived += received;
    }

    return true;
}

bool Network::netConnect() {
    if (mConnected) {
        return true;
    }

    // TODO support ipv4
    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "unable to create socket: " << strerror(errno) << "\n";
        return false;
    }
    mSock = sock;

    // TODO move this into context
    std::string host = mContext->address;
    std::string port = "1883";
    size_t pos = mContext->address.find_last_of(':');
    if (pos != std::string::npos) {
        host = mContext->address.substr(0, pos);
        port = mContext->address.substr(pos + 1);
    }

    // TODO resolve localhost, hosts
    sockaddr_in6 server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(std::stoi(port));
    if (inet_pton(AF_INET6, host.c_str(), &server_addr.sin6_addr) <= 0) {
        netClose();
        std::cerr << "unable to create address: " << strerror(errno) << "\n";
        return false;
    }

    if (connect(mSock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        netClose();
        std::cerr << "unable to connect to server: " << strerror(errno) << "\n";
        return false;
    }

    std::cout << "connected\n";
    mConnected = true;
    return true;
}

void Network::netClose() {
    if (mSock != 0) {
        close(mSock);
        mConnected = false;
    }
}
} // namespace MqttClient