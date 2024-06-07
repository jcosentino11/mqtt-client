#pragma once

#include <string>

namespace MqttClient {

struct Context {
    std::string command;
    std::string topic;
    std::string message;
    std::string address;
    std::string clientId;
    bool verbose;
};

enum ParseResultCode { SUCCESS, FAILURE, HELP };

struct ParseResult {
    ParseResultCode code;
    std::string error;
    Context context;
};

ParseResult parseContext(int argc, char *argv[]) noexcept;

const std::string USAGE_PUB = R"(
Usage: mqtt pub [-v] -t <topic> -a <address> -c <clientId> <message>
Options:
  -t, --topic <topic>           Specify the topic to publish to (required)
  -a, --address <address>       <host>[:<port>] address to connect to (required)
  -c, --client-id <client id>   Client identifier
  -v, --verbose                 Enable verbose output
  -h, --help                    Show this help message

Example:
  mqtt pub -t hello/topic -v -a 127.0.0.1:8883 -c myclient '{"hello": "world"}'
)";

const std::string USAGE_SUB = R"(
Usage: mqtt sub [-v] -t <topic> -a <address> -c <clientId>
Options:
  -t, --topic <topic>           Specify the topic to publish to (required)
  -a, --address <address>       <host>[:<port>] address to connect to (required)
  -c, --client-id <client id>   Client identifier
  -v, --verbose                 Enable verbose output
  -h, --help                    Show this help message

Example:
  mqtt sub -t hello/topic -v -a 127.0.0.1:8883 -c myclient
)";

}; // namespace MqttClient