#include "Command.h"
#include "Context.h"
#include "Interrupt.h"
#include <csignal>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <string>

using namespace MqttClient;

int main(int argc, char *argv[]) {
    if (signal(SIGINT, onInterrupt) == SIG_ERR) {
        std::cerr << "Unable to register SIGINT handler\n";
        return 1;
    }
    if (signal(SIGTERM, onInterrupt) == SIG_ERR) {
        std::cerr << "Unable to register SIGTERM handler\n";
        return 1;
    }

    auto res = parseContext(argc, argv);
    switch (res.code) {
    case ParseResultCode::HELP:
        if (res.context.command == "sub") {
            std::cout << USAGE_SUB;
        } else {
            std::cout << USAGE_PUB;
        }
        return 0;
    case ParseResultCode::FAILURE:
        std::cerr << res.error << "\n";
        if (res.context.command == "sub") {
            std::cout << USAGE_SUB;
        } else {
            std::cout << USAGE_PUB;
        }
        return 1;
    case ParseResultCode::SUCCESS:
        // carry on
        break;
    }

    auto context = res.context;

    if (context.verbose) {
        std::cout << "Context[command=" << context.command
                  << ", topic=" << context.topic
                  << ", address=" << context.address
                  << ", clientId=" << context.clientId
                  << ", message=" << context.message << "]\n";
    }

    auto command = new Command(std::make_shared<Context>(context));
    command->execute();

    return 0;
}
