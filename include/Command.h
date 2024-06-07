#pragma once

#include "Context.h"
#include "Network.h"
#include "Packet.h"
#include <memory>

namespace MqttClient {

class Command {
  public:
    Command(std::shared_ptr<Context> context);
    void execute();

  private:
    std::shared_ptr<Context> mContext;
    PacketBuilder mPacketBuilder;
    Network mNetwork;

    bool connect();
    bool publish();
    bool subscribe();
};
} // namespace MqttClient