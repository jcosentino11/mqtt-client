#include "Interrupt.h"
#include <atomic>

namespace MqttClient {

std::atomic<bool> interrupted(false);

void onInterrupt(int ignored) { interrupted.store(true); }
} // namespace MqttClient
