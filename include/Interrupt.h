#pragma once
#include <atomic>

namespace MqttClient {
extern std::atomic<bool> interrupted;
void onInterrupt(int ignored);
} // namespace MqttClient
