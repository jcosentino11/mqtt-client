#pragma once
#include "Context.h"
#include <memory>

namespace MqttClient {

class Network {
  public:
    Network(std::shared_ptr<Context> context);

  private:
    std::shared_ptr<Context> mContext;
};

} // namespace MqttClient