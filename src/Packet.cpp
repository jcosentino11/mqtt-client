#include "Packet.h"
#include "Context.h"
#include <memory>

namespace MqttClient {
PacketBuilder::PacketBuilder(std::shared_ptr<Context> context)
    : mContext(std::move(context)) {};

bool PacketBuilder::connect(Payload &payload) {
    char variableHeaderLength = 10; // TODO const

    // client id length needs to fit in the second byte of the connect
    // packet (remainingLength)
    if (mContext->clientId.size() >
        255 - variableHeaderLength) { // TODO can this just be moved to
                                      // earlier validation?
        return false;                 // TODO error handling
    }
    char remainingLength =
        variableHeaderLength +
        (2 + mContext->clientId.size()); // 2 two encode length

    size_t connPacketLen = 2 + remainingLength;
    payload.reserve(connPacketLen);

    // supported payload fields: client identifier
    // TODO will topic, will message, user name, password, clean session

    // BEGIN FIXED HEADER
    payload.push_back(0b00010000); // MQTT control packet type (1)
    payload.push_back(remainingLength);
    // BEGIN VARIABLE HEADER
    payload.push_back(0); // MQTT length MSB
    payload.push_back(4); // MQTT length LSB
    payload.push_back('M');
    payload.push_back('Q');
    payload.push_back('T');
    payload.push_back('T');
    payload.push_back(4); // protocol level (MQTT 3.1.1)
    payload.push_back(
        0b00000010);      // connect flags, just cleanSession=true for now
    payload.push_back(0); // keep alive MSB
    payload.push_back(0); // keep alive LSB
    // BEGIN PAYLOAD
    // client id
    payload.push_back(0);                         // TODO client id length MSB
    payload.push_back(mContext->clientId.size()); // client id length LSB
    for (size_t c = 0; c < mContext->clientId.size(); ++c) {
        payload.push_back(mContext->clientId[c]);
    }
    return true;
}
} // namespace MqttClient