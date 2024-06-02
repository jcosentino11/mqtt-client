#include "Command.h"
#include "Context.h"
#include <cstring>
#include <iostream>

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

    if (mContext.command == "pub") {

        // TODO move connect packet to separate class

        char variableHeaderLength = 10; // TODO const

        // client id length needs to fit in the second byte of the connect
        // packet (remainingLength)
        if (mContext.clientId.size() >
            255 - variableHeaderLength) { // TODO can this just be moved to
                                          // earlier validation?
            return;                       // TODO error handling
        }
        char remainingLength = variableHeaderLength + mContext.clientId.size();

        // supported payload fields: client identifier
        // TODO will topic, will message, user name, password, clean session
        size_t i = 0;

        char connectPacket[2 + remainingLength];
        // BEGIN FIXED HEADER
        connectPacket[i++] = 0b00010000; // MQTT control packet type (1)
        connectPacket[i++] = remainingLength;
        // BEGIN VARIABLE HEADER
        connectPacket[i++] = 0; // length of MSB
        connectPacket[i++] = 4; // length of LSB
        connectPacket[i++] = 'M';
        connectPacket[i++] = 'Q';
        connectPacket[i++] = 'T';
        connectPacket[i++] = 'T';
        connectPacket[i++] = 4;          // protocol level (MQTT 3.1.1)
        connectPacket[i++] = 0b00000000; // connect flags
        connectPacket[i++] = 0;          // keep alive MSB
        connectPacket[i++] = 0;          // keep alive LSB
        // BEGIN PAYLOAD
        // client id
        for (size_t c = 0; c < mContext.clientId.size(); ++c) {
            connectPacket[i++] = mContext.clientId[c];
        }

        // TODO move network stuff to separate class
        // TODO support ipv4
        // TODO resolve localhost, hosts

        int sock = socket(AF_INET6, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "unable to create socket: " << strerror(errno) << "\n";
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
        if (inet_pton(AF_INET6, host.c_str(), &server_addr.sin6_addr) <= 0) {
            close(sock);
            std::cerr << "unable to create address: " << strerror(errno)
                      << "\n";
            return;
        }

        if (connect(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            close(sock);
            std::cerr << "unable to connect to server: " << strerror(errno)
                      << "\n";
            return;
        }

        std::cout << "connected\n";

        if (send(sock, mContext.message.data(), mContext.message.size(), 0) <
            0) {
            close(sock);
            std::cerr << "unable to send message: " << strerror(errno) << "\n";
            return; // TODO error handling
        }

        std::cout << "CONNECT packet sent\n";

        // TODO listen to CONNACK
    }
}

} // namespace MqttClient
