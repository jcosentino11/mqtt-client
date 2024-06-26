#include "Packet.h"
#include "Context.h"
#include <memory>
#include <random>

namespace MqttClient {
PacketBuilder::PacketBuilder(std::shared_ptr<Context> context)
    : mContext(std::move(context)) {};

// TODO will topic, will message, user name, password, clean session
// TODO support MQTT5
bool PacketBuilder::connect(Payload &payload) {
    // BEGIN FIXED HEADER
    // ------------------
    // MQTT control packet type (1)
    payload.push_back(0b00010000);
    // remaining length
    payload.push_back(10 + (2 + mContext->clientId.size()));

    // BEGIN VARIABLE HEADER
    // ---------------------
    // MQTT length MSB
    payload.push_back(0);
    // MQTT length LSB
    payload.push_back(4);
    payload.push_back('M');
    payload.push_back('Q');
    payload.push_back('T');
    payload.push_back('T');
    // protocol level (MQTT 3.1.1)
    payload.push_back(4);
    // connect flags
    //    cleanSession=true
    // TODO support other flags
    payload.push_back(0b00000010);
    // keep alive MSB
    // TODO
    payload.push_back(0);
    // keep alive LSB
    // TODO
    payload.push_back(0);

    // BEGIN PAYLOAD
    // -------------
    // client id length MSB
    // TODO
    payload.push_back(0);
    // client id length LSB
    payload.push_back(mContext->clientId.size());
    // client id
    for (size_t i = 0; i < mContext->clientId.size(); ++i) {
        payload.push_back(mContext->clientId[i]);
    }

    return true;
}

bool PacketBuilder::publish(Payload &payload) {
    // BEGIN FIXED HEADER
    // ------------------
    // MQTT control packet type (3)
    // TODO support dup, retain
    auto controlPacket = 0b00110000;
    // set qos
    controlPacket = controlPacket | mContext->qos << 1;
    payload.push_back(controlPacket);
    // remaining length
    uint8_t remainingLength =
        (2 + mContext->topic.size()) + mContext->message.size();
    if (mContext->qos > 0) {
        remainingLength += 2; // packet identifier
    }
    payload.push_back(remainingLength);

    // BEGIN VARIABLE HEADER
    // ---------------------
    // topic length MSB
    // TODO
    payload.push_back(0);
    // topic length LSB
    payload.push_back(mContext->topic.size());
    // topic
    for (size_t i = 0; i < mContext->topic.size(); ++i) {
        payload.push_back(mContext->topic[i]);
    }
    if (mContext->qos > 0) {
        // packet identifier MSB
        // TODO
        payload.push_back(0);
        // packet identifier LSB
        payload.push_back(packetId());
    }

    // BEGIN PAYLOAD
    // -------------
    for (size_t i = 0; i < mContext->message.size(); ++i) {
        payload.push_back(mContext->message[i]);
    }

    return true;
}

bool PacketBuilder::subscribe(Payload &payload) {
    // BEGIN FIXED HEADER
    // ------------------
    // MQTT control packet type (8)
    payload.push_back(0b10000010);
    // remaining length
    payload.push_back(2 + (3 + mContext->topic.size()));

    // BEGIN VARIABLE HEADER
    // ---------------------
    // packet identifier MSB
    // TODO
    payload.push_back(0);
    // packet identifier LSB
    payload.push_back(packetId());

    // BEGIN PAYLOAD
    // -------------
    // topic length MSB
    // TODO
    payload.push_back(0);
    // topic length LSB
    payload.push_back(mContext->topic.size());
    // topic
    for (size_t i = 0; i < mContext->topic.size(); ++i) {
        payload.push_back(mContext->topic[i]);
    }
    // topic QoS
    payload.push_back(mContext->qos);

    return true;
}

uint16_t PacketBuilder::packetId() {
    // TODO rework this
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(200, 500);
    return distribution(gen);
}

} // namespace MqttClient