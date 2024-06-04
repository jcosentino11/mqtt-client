#pragma once

#include "Context.h"
#include "Network.h"
#include <memory>
#include <vector>

namespace MqttClient {

class PacketBuilder {

  public:
    PacketBuilder(std::shared_ptr<Context> context);
    bool connect(Payload &payload); // TODO more specific error handling?
    bool publish(Payload &payload);

  private:
    std::shared_ptr<Context> mContext;
};
} // namespace MqttClient