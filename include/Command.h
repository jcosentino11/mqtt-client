#pragma once

#include "Context.h"
#include <memory>

namespace MqttClient {

class Command {
  public:
    Command(std::shared_ptr<Context> context);
    void execute();

  private:
    std::shared_ptr<Context> mContext;
};
} // namespace MqttClient