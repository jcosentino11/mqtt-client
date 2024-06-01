#include "Context.h"
#include <getopt.h>
#include <iostream>
#include <string>

namespace MqttClient {
namespace Context {
ParseResult parse(int argc, char *argv[]) {
    if (argc < 2) {
        return {ParseResultStatus::FAILURE};
    }

    Context context;
    context.command = argv[1];

    if (context.command != "pub") {
        return {ParseResultStatus::FAILURE,
                "Unknown command: " + context.command};
    }

    // discard the command
    argc -= 1;
    argv += 1;

    struct option long_options[] = {{"topic", required_argument, 0, 't'},
                                    {"verbose", no_argument, 0, 'v'},
                                    {"help", no_argument, 0, 'h'},
                                    {0, 0, 0, 0}};

    int option_index = 0;
    int option;
    while ((option = getopt_long(argc, argv, "t:vh", long_options,
                                 &option_index)) != -1) {
        switch (option) {
        case 't':
            context.topic = optarg;
            break;
        case 'v':
            context.verbose = true;
            break;
        case 'h':
            std::cout << USAGE;
            return {ParseResultStatus::HELP};
        case '?':
            std::cout << USAGE;
            return {ParseResultStatus::FAILURE};
        default:
            std::cout << USAGE;
            return {ParseResultStatus::FAILURE};
        }
    }

    if (context.topic.empty()) {
        // TODO more validations
        return {ParseResultStatus::FAILURE, "Error: Topic is required"};
    }

    if (optind == argc) {
        return {ParseResultStatus::FAILURE, "Error: Message is required"};
    }

    auto numMessages = argc - optind;
    if (numMessages > 1) {
        std::cerr << "Error: " << numMessages
                  << " messages were provided. Currently, only 1 message at a "
                     "time is supported\n";
        std::cout << USAGE;
        return {ParseResultStatus::FAILURE,
                "Error: " + std::to_string(numMessages) +
                    " messages were provided. Currently, only 1 message at a "
                    "time is supported"};
    }

    context.message = argv[optind];

    return {ParseResultStatus::SUCCESS, {}, context};
}
} // namespace Context
} // namespace MqttClient