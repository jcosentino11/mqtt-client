#pragma once

#include <string>

namespace MqttClient {

struct Context {
    std::string command;
    std::string topic;
    std::string message;
    std::string address;
    bool verbose;
};

enum ParseResultCode { SUCCESS, FAILURE, HELP };

struct ParseResult {
    ParseResultCode code;
    std::string error;
    Context context;
};

ParseResult parseContext(int argc, char *argv[]) noexcept;

const std::string USAGE = R"(
Usage: mqtt pub [-v] -t <topic> -a <address> <message>
Options:
  -t, --topic <topic>      Specify the topic to publish to (required)
  -a, --address <address>  <host>[:<port>] address to connect to (required)
  -v, --verbose            Enable verbose output
  -h, --help               Show this help message

Example:
  mqtt pub -t hello/topic -v -a 127.0.0.1:8883 '{"hello": "world"}'
)";

}; // namespace MqttClient