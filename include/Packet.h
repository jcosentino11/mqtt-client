#pragma once

#include "Context.h"
#include <vector>

namespace MqttClient {

using Payload = std::vector<char>;

class PacketBuilder {

  public:
    PacketBuilder(Context &context);
    bool connect(Payload &payload); // TODO more specific error handling?

  private:
    Context mContext;
};
} // namespace MqttClient