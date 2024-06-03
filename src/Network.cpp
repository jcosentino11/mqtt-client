#include "Network.h"
#include "Command.h"
#include "Context.h"
#include "Packet.h"
#include <cstring>
#include <iostream>
#include <vector>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace MqttClient {
Network::Network(std::shared_ptr<Context> context)
    : mContext(std::move(context)) {};
}