#pragma once

#include "Context.h"
#include <memory>
#include <vector>

namespace MqttClient {

using Payload = std::vector<char>;

class PacketBuilder {

  public:
    PacketBuilder(std::shared_ptr<Context> context);
    bool connect(Payload &payload); // TODO more specific error handling?

  private:
    std::shared_ptr<Context> mContext;
};
} // namespace MqttClient