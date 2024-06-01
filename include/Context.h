#pragma once

#include <string>

namespace MqttClient {

struct Context {
    std::string command;
    std::string topic;
    std::string message;
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
Usage: mqtt pub -t <topic> [-v] <message>
Options:
  -t, --topic <topic>   Specify the topic to publish to
  -v, --verbose         Enable verbose output
  -h, --help            Show this help message

Example:
  mqtt pub -t hello/topic -v '{"hello": "world"}'
)";

}; // namespace MqttClient