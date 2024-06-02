#include "Command.h"
#include "Context.h"
#include <getopt.h>
#include <iostream>
#include <string>

using namespace MqttClient;

int main(int argc, char *argv[]) {
    auto res = parseContext(argc, argv);
    switch (res.code) {
    case ParseResultCode::HELP:
        std::cout << USAGE;
        return 0;
    case ParseResultCode::FAILURE:
        std::cerr << res.error << "\n";
        std::cout << USAGE;
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

    auto command = new Command(context);
    command->execute();

    return 0;
}
