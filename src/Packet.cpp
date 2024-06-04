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

    // supported payload fields: client identifier
    // TODO will topic, will message, user name, password, clean session
    // TODO support MQTT5

    // BEGIN FIXED HEADER
    payload.push_back(0b00010000); // MQTT control packet type (1)
    payload.push_back(variableHeaderLength + 2 +
                      mContext->clientId.size()); // remainingLength
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
    for (size_t i = 0; i < mContext->clientId.size(); ++i) {
        payload.push_back(mContext->clientId[i]);
    }
    return true;
}

bool PacketBuilder::publish(Payload &payload) {
    // BEGIN FIXED HEADER
    payload.push_back(0b00110000); // MQTT control packet type (3) TODO support
                                   // dup , qos, retain
    payload.push_back(2 + mContext->topic.size() +
                      mContext->message.size()); // remainingLength
    // BEGIN VARIABLE HEADER
    payload.push_back(0);                      // TODO topic length MSB
    payload.push_back(mContext->topic.size()); // topic length LSB
    for (size_t i = 0; i < mContext->topic.size(); ++i) {
        payload.push_back(mContext->topic[i]);
    }
    for (size_t i = 0; i < mContext->message.size(); ++i) {
        payload.push_back(mContext->message[i]);
    }
    return true;
}
} // namespace MqttClient