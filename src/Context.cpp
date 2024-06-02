#include "Context.h"
#include <getopt.h>
#include <iostream>
#include <string>

namespace MqttClient {

ParseResult parseContext(int argc, char *argv[]) noexcept {
    if (argc < 2) {
        return {ParseResultCode::FAILURE};
    }

    Context context;
    context.command = argv[1];

    struct option opts[] = {{"topic", required_argument, 0, 't'},
                            {"address", required_argument, 0, 'a'},
                            {"verbose", no_argument, 0, 'v'},
                            {"help", no_argument, 0, 'h'},
                            {0, 0, 0, 0}};
    int opt;
    int optInd = 0;
    while ((opt = getopt_long(argc, argv, "t:a:vh", opts, &optInd)) != -1) {
        switch (opt) {
        case 't':
            context.topic = optarg;
            break;
        case 'a':
            context.address = optarg;
            break;
        case 'v':
            context.verbose = true;
            break;
        case 'h':
            return {ParseResultCode::HELP};
        case '?':
            return {ParseResultCode::FAILURE};
        default:
            return {ParseResultCode::FAILURE};
        }
    }

    if (context.command != "pub") {
        return {ParseResultCode::FAILURE,
                "Unknown command: " + context.command};
    }

    if (context.topic.empty()) {
        // TODO more validations
        return {ParseResultCode::FAILURE, "Error: Topic is required"};
    }

    if (context.address.empty()) {
        // TODO more validations
        return {ParseResultCode::FAILURE, "Error: Address is required"};
    }

    auto numMessages = argc - 1 - optind;
    if (numMessages == 0) {
        return {ParseResultCode::FAILURE, "Error: Message is required"};
    }
    if (numMessages > 1) {
        return {ParseResultCode::FAILURE,
                "Error: " + std::to_string(numMessages) +
                    " messages were provided. Currently, only 1 message at a "
                    "time is supported"};
    }

    context.message = argv[optind + 1];

    return {ParseResultCode::SUCCESS, {}, context};
}

} // namespace MqttClient