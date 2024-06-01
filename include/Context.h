#pragma once

#include <string>

namespace MqttClient {

namespace Context {
struct Context {
    std::string command;
    std::string topic;
    std::string message;
    bool verbose;
};

enum ParseResultStatus { SUCCESS, FAILURE, HELP };

struct ParseResult {
    ParseResultStatus status;
    std::string error;
    Context context;
};

ParseResult parse(int argc, char *argv[]);

const std::string USAGE = R"(
Usage: mqtt pub -t <topic> [-v] <message>
Options:
  -t, --topic <topic>   Specify the topic to publish to
  -v, --verbose         Enable verbose output
  -h, --help            Show this help message

Example:
  mqtt pub -t hello/topic -v '{"hello": "world"}'
)";
} // namespace Context

}; // namespace MqttClient