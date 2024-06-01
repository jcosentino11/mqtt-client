// for windows compatibility
#include "getopt.h"
#include <iostream>
#include <string>

void print_usage() {
    std::cout << "Usage: mqtt pub -t <topic> [-v] <message>\n"
              << "Options:\n"
              << "  -t, --topic <topic>   Specify the topic to publish to\n"
              << "  -v, --verbose         Enable verbose output\n"
              << "  -h, --help            Show this help message\n"
              << "\n"
              << "Example:\n"
              << "  mqtt pub -t hello/topic -v '{\"hello\": \"world\"}'\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command;
    std::string topic;
    std::string message;
    bool verbose = false;

    command = argv[1];

    if (command != "pub") {
        std::cerr << "Unknown command: " << command << "\n";
        print_usage();
        return 1;
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
            topic = optarg;
            break;
        case 'v':
            verbose = true;
            break;
        case 'h':
            print_usage();
            return 0;
        case '?':
            print_usage();
            return 1;
        default:
            print_usage();
            return 1;
        }
    }

    if (topic.empty()) {
        // TODO more validations
        std::cerr << "Error: Topic is required\n";
        print_usage();
        return 1;
    }

    if (optind == argc) {
        std::cerr << "Error: Message is required\n";
        print_usage();
        return 1;
    }

    auto numMessages = argc - optind;
    if (numMessages > 1) {
        std::cerr << "Error: " << numMessages
                  << " messages were provided. Currently, only 1 message at a "
                     "time is supported\n";
        print_usage();
        return 1;
    }

    message = argv[optind];

    if (verbose) {
        // TODO have commands log their inputs
        std::cout << "Command: " << command << "\n";
        std::cout << "Topic: " << topic << "\n";
        std::cout << "Message: " << message << "\n";
    }

    // TODO implement commands

    return 0;
}
