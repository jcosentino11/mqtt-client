#include "Context.h"
#include <getopt.h>
#include <iostream>
#include <string>

using namespace MqttClient::Context;

int main(int argc, char *argv[]) {
    auto parseResult = parse(argc, argv);
    switch (parseResult.status) {
    case ParseResultStatus::HELP:
        std::cout << USAGE;
        return 0;
    case ParseResultStatus::FAILURE:
        std::cerr << parseResult.error << "\n";
        std::cout << USAGE;
        return 1;
    case ParseResultStatus::SUCCESS:
        // carry on
    }

    auto context = parseResult.context;

    if (context.verbose) {
        std::cout << "Context[command=" << context.command
                  << ", topic=" << context.topic
                  << ", message=" << context.message << "]\n";
    }

    // TODO implement commands

    return 0;
}
