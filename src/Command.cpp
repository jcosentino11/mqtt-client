#include "Command.h"
#include "Context.h"
#include <iostream>

namespace MqttClient {

Command::Command(Context context) : mContext(context) {}

void Command::execute() {
    if (mContext.verbose) {
        std::cout << "Executing " << mContext.command << " command\n";
    }

    if (mContext.command == "pub") {

        char variableHeaderLength = 10; // TODO const

        // client id length needs to fix in the second byte of the connect
        // packet (remainingLength)
        if (mContext.clientId.size() > 255 - variableHeaderLength) {
            return; // TODO error handling
        }
        char remainingLength = variableHeaderLength + mContext.clientId.size();

        // supported payload fields: client identifier
        // TODO will topic, will message, user name, password, clean session
        size_t i;

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

        // TODO send the packet
    }
}

} // namespace MqttClient
