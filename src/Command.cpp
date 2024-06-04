#include "Command.h"
#include "Context.h"
#include "Packet.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace MqttClient {

Command::Command(std::shared_ptr<Context> context)
    : mContext(std::move(context)), mPacketBuilder(mContext),
      mNetwork(mContext) {}

void Command::execute() {
    if (mContext->verbose) {
        std::cout << "Executing " << mContext->command << " command\n";
    }

    if (!connect()) {
        return;
    }

    if (mContext->command == "pub") {
        publish();
    } else if (mContext->command == "sub") {
        // TODO
    }
}

bool Command::connect() {
    Payload conn;
    if (!mPacketBuilder.connect(conn)) {
        std::cerr << "Unable to build CONNECT packet\n";
        return false;
    }

    if (!mNetwork.netSend(conn)) {
        std::cerr << "Unable to send CONNECT: " << strerror(errno) << "\n";
        return false;
    }

    if (mContext->verbose) {
        std::cout << "CONNECT sent\n";
    }

    Payload connAck;
    if (!mNetwork.netRecv(connAck, 4) || connAck[0] != 0b00100000) {
        std::cerr << "CONNACK not received\n";
        return false;
    }

    bool sessionPresent = connAck[2];
    int returnCode = connAck[3];

    if (mContext->verbose) {
        std::cout << "CONNACK received! return code: " << returnCode
                  << ", session present: " << sessionPresent << "\n";
    }
    return true;
}

bool Command::publish() {
    Payload publish;
    if (!mPacketBuilder.publish(publish)) {
        std::cerr << "Unable to build PUBLISH packet\n";
        return false;
    }

    if (!mNetwork.netSend(publish)) {
        std::cerr << "Unable to send PUBLISH: " << strerror(errno) << "\n";
        return false;
    }

    if (mContext->verbose) {
        std::cout << "PUBLISH sent\n";
    }

    return true;

    // PUBACK
}

} // namespace MqttClient
