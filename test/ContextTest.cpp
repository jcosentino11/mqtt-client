
#include "Context.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace MqttClient;

class ContextTest : public testing::Test {
  public:
    ParseResult parse(std::vector<std::string> args) {
        if (args.empty()) {
            return parseContext(0, {});
        }
        std::vector<char *> argv;
        for (size_t i = 0; i < args.size(); ++i) {
            argv.push_back(const_cast<char *>(args[i].c_str()));
        }
        return parseContext(argv.size(), &argv[0]);
    }
};

TEST_F(ContextTest, AllOpts) {
    auto res = parse({"mqtt", "pub", "-v", "-t", "topic", "-a",
                      "127.0.0.1:8883", "-c", "client", "message"});
    EXPECT_EQ(ParseResultCode::SUCCESS, res.code);
}

TEST_F(ContextTest, RequiredOpts) {
    auto res = parse({"mqtt", "pub", "-t", "topic", "-a", "127.0.0.1:8883",
                      "-c", "client", "message"});
    EXPECT_EQ(ParseResultCode::SUCCESS, res.code);
}

TEST_F(ContextTest, Help) {
    auto res = parse({"mqtt", "-h"});
    EXPECT_EQ(ParseResultCode::HELP, res.code);
}

TEST_F(ContextTest, NoArgs) {
    auto res = parse({});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingMessage) {
    auto res = parse(
        {"mqtt", "pub", "-t", "topic", "-a", "127.0.0.1:8883", "-c", "client"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingTopic) {
    auto res = parse(
        {"mqtt", "pub", "-a", "127.0.0.1:8883", "-c", "client", "message"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingTopicValue) {
    auto res = parse({"mqtt", "pub", "-t", "-a", "127.0.0.1:8883", "-c",
                      "client", "message"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingAddress) {
    auto res = parse({"mqtt", "pub", "-t", "topic", "-c", "client", "message"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingAddressValue) {
    auto res =
        parse({"mqtt", "pub", "-t", "topic", "-a", "-c", "client", "message"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MultipleMessages) {
    auto res = parse({"mqtt", "pub", "-v", "-t", "topic", "-a",
                      "127.0.0.1:8883", "-c", "client", "message", "message2"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingClientId) {
    auto res = parse({"mqtt", "pub", "-v", "-t", "topic", "-a",
                      "127.0.0.1:8883", "message"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, MissingClientIdValue) {
    auto res = parse({"mqtt", "pub", "-v", "-t", "topic", "-a",
                      "127.0.0.1:8883", "-c", "message"});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}
