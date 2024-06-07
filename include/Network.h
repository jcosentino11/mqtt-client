#pragma once
#include "Context.h"
#include <memory>
#include <vector>

namespace MqttClient {

using Payload = std::vector<uint8_t>;

class Network {
  public:
    Network(std::shared_ptr<Context> context);
    ~Network();

    bool netSend(Payload &payload);
    bool netRecv(Payload &payload, size_t len);

  private:
    std::shared_ptr<Context> mContext;
    int mSock;
    bool mConnected;

    bool netConnect();
    void netClose();
};

} // namespace MqttClient