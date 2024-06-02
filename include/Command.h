#pragma once

#include "Context.h"

namespace MqttClient {

class Command {
  public:
    Command(Context context);
    void execute();

  private:
    Context mContext;
};
} // namespace MqttClient