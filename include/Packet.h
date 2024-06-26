#pragma once

#include "Context.h"
#include "Network.h"
#include <memory>
#include <vector>

namespace MqttClient {

class PacketBuilder {

  public:
    PacketBuilder(std::shared_ptr<Context> context);
    bool connect(Payload &payload);
    bool publish(Payload &payload);
    bool subscribe(Payload &payload);

  private:
    std::shared_ptr<Context> mContext;
    uint16_t packetId();
};
} // namespace MqttClient