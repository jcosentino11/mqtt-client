#include "Command.h"
#include "Context.h"
#include <iostream>

namespace MqttClient {

Command::Command(Context context) : mContext(context) {}

void Command::execute() {
    if (mContext.verbose) {
        std::cout << "Executing " << mContext.command
                  << " command\n"; // TODO display actual packet name
    }
}

} // namespace MqttClient
