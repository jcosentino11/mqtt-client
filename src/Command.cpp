#include "Command.h"
#include "Context.h"
#include "Interrupt.h"
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
        std::cout << "Executing " << mContext->command << " command"
                  << std::endl;
    }

    if (!connect()) {
        return;
    }

    if (mContext->command == "pub") {
        publish();
    } else if (mContext->command == "sub") {
        subscribe();
    }
}

bool Command::connect() {
    Payload conn;
    if (!mPacketBuilder.connect(conn)) {
        std::cerr << "Unable to build CONNECT packet" << std::endl;
        return false;
    }

    if (!mNetwork.netSend(conn)) {
        std::cerr << "Unable to send CONNECT: " << strerror(errno) << ""
                  << std::endl;
        return false;
    }

    if (mContext->verbose) {
        std::cout << "CONNECT sent" << std::endl;
    }

    Payload connAck;
    if (!mNetwork.netRecv(connAck, 4) || connAck[0] != 0b00100000) {
        std::cerr << "CONNACK not received" << std::endl;
        return false;
    }

    bool sessionPresent = connAck[2];
    int returnCode = connAck[3];

    if (mContext->verbose) {
        std::cout << "CONNACK received! return code: " << returnCode
                  << ", session present: " << sessionPresent << "" << std::endl;
    }
    return true;
}

bool Command::publish() {
    Payload publish;
    if (!mPacketBuilder.publish(publish)) {
        std::cerr << "Unable to build PUBLISH packet" << std::endl;
        return false;
    }

    if (!mNetwork.netSend(publish)) {
        std::cerr << "Unable to send PUBLISH: " << strerror(errno) << ""
                  << std::endl;
        return false;
    }

    if (mContext->verbose) {
        std::cout << "PUBLISH sent" << std::endl;
    }

    // TODO support QoS 2
    if (mContext->qos == 1) {
        Payload pubAck;
        if (!mNetwork.netRecv(pubAck, 4)) {
            std::cerr << "PUBACK not received" << std::endl;
            return false;
        }
        if (pubAck[0] != 0b01000000) {
            std::cerr << "Another packet received instead of PUBACK"
                      << std::endl;
            return false;
        }
        if (mContext->verbose) {
            std::cout << "PUBACK received" << std::endl;
        }
    }

    return true;
}

bool Command::subscribe() {
    Payload subscribe;
    if (!mPacketBuilder.subscribe(subscribe)) {
        std::cerr << "Unable to build SUBSCRIBE packet" << std::endl;
        return false;
    }

    if (!mNetwork.netSend(subscribe)) {
        std::cerr << "Unable to send SUBSCRIBE: " << strerror(errno) << ""
                  << std::endl;
        return false;
    }

    if (mContext->verbose) {
        std::cout << "SUBSCRIBE sent" << std::endl;
    }

    Payload subAck;
    if (!mNetwork.netRecv(subAck, 5)) {
        std::cerr << "SUBACK not received" << std::endl;
        return false;
    }

    auto packetType = subAck[0];
    if (packetType != 0b10010000) {
        std::cerr << "Another packet received instead of SUBACK" << std::endl;
        return false;
    }

    if (subAck[4] != mContext->qos) {
        std::cerr << "Subscription failed" << std::endl;
        return false;
    }

    if (mContext->verbose) {
        std::cout << "SUBACK received" << std::endl;
    }

    // receive PUBLISH messages until interrupted
    while (!interrupted.load()) {
        uint8_t qos;
        {
            Payload packet;
            if (!mNetwork.netRecv(packet, 1)) {
                if (mContext->verbose) {
                    std::cerr << "Unable to receive PUBLISH packet"
                              << std::endl;
                }
                continue;
            }
            if (interrupted.load()) {
                return true;
            }
            packetType = packet[0] & 0b11110000;
            qos = packet[0] & 0b00000110;

            if (packetType != 0b00110000) {
                if (mContext->verbose) {
                    std::cerr << "Unexpected packet type received";
                }
                continue;
            }
        }

        uint8_t remainingLen;
        {
            Payload packet;
            if (!mNetwork.netRecv(packet, 1)) {
                if (mContext->verbose) {
                    std::cerr << "Unable to receive remaining length"
                              << std::endl;
                }
                continue;
            }
            if (interrupted.load()) {
                return true;
            }
            remainingLen = packet[0];
        }

        std::string message;
        {
            Payload packet;
            if (!mNetwork.netRecv(packet, remainingLen)) {
                if (mContext->verbose) {
                    std::cerr << "Unable to receive rest of PUBLISH packet"
                              << std::endl;
                }
                continue;
            }
            if (interrupted.load()) {
                return true;
            }

            int messageStart = 0;

            uint16_t topicLen = packet[1] + (packet[0] << 8);
            messageStart += 2;
            messageStart += topicLen;

            if (qos > 0) {
                messageStart += 2; // packet id
            }

            if (messageStart == packet.size()) {
                message = "";
            } else if (messageStart < packet.size()) {
                message =
                    std::string(packet.begin() + messageStart, packet.end());
            } else {
                if (mContext->verbose) {
                    std::cerr << "Not enough buffer for message payload"
                              << std::endl;
                }
                continue;
            }
        }

        std::cout << message << std::endl;
    }

    return true;
}

} // namespace MqttClient
