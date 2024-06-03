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
    : mContext(std::move(context)) {}

void Command::execute() {
    if (mContext->verbose) {
        std::cout << "Executing " << mContext->command << " command\n";
    }

    auto packetBuilder = PacketBuilder(mContext);
    auto network = Network(mContext);

    // TODO this needs major cleanup
    if (mContext->command == "pub") {

        Payload conn;
        if (!packetBuilder.connect(conn)) {
            std::cerr << "Unable to build CONNECT packet";
            return;
        }

        if (!network.netSend(conn)) {
            std::cerr << "unable to send CONNECT: " << strerror(errno) << "\n";
            return;
        }

        Payload connAck;
        if (!network.netRecv(connAck, 4) || connAck[0] != 0b00100000) {
            std::cerr << "CONNACK not received";
            return;
        }

        bool sessionPresent = connAck[2];
        int returnCode = connAck[3];
        std::cout << "CONNACK received! return code: " << returnCode
                  << ", session present: " << sessionPresent << "\n";
    }
}

} // namespace MqttClient
