#include "Network.h"
#include "Command.h"
#include "Context.h"
#include "Packet.h"
#include <cstring>
#include <iostream>
#include <vector>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace MqttClient {
Network::Network(std::shared_ptr<Context> context)
    : mContext(std::move(context)), mSock{-1}, mConnected{false} {};
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

    payload.resize(len);

    auto totalReceived = 0;
    while (totalReceived < len) {
        auto received =
            recv(mSock, payload.data() + totalReceived, len - totalReceived, 0);
        if (received <= 0) {
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

    // TODO move this into context
    std::string host = mContext->address;
    std::string port = "1883";
    size_t pos = mContext->address.find_last_of(':');
    if (pos != std::string::npos) {
        host = mContext->address.substr(0, pos);
        port = mContext->address.substr(pos + 1);
    }

    struct addrinfo hints = {}, *addrs;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (getaddrinfo(host.data(), port.data(), &hints, &addrs) != 0) {
        std::cerr << "unable to resolve address: " << strerror(errno) << ""
                  << std::endl;
        return false;
    }

    for (struct addrinfo *addr = addrs; addr != NULL; addr = addr->ai_next) {
        mSock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (mSock == -1) {
            continue;
        }

        if (connect(mSock, addr->ai_addr, addr->ai_addrlen) < 0) {
            std::cerr << "unable to connect to host: " << strerror(errno) << ""
                      << std::endl;
            freeaddrinfo(addrs);
            netClose();
            return false;
        }
    }

    // socket failed for all addrs
    if (mSock < 0) {
        freeaddrinfo(addrs);
        netClose();
        return false;
    }

    mConnected = true;
    return true;
}

void Network::netClose() {
    if (mSock >= 0) {
        close(mSock);
        mConnected = false;
        mSock = -1;
    }
}
} // namespace MqttClient