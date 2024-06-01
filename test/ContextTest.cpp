
#include "Context.h"
#include <gtest/gtest.h>

using namespace MqttClient;

class ContextTest : public testing::Test {
  public:
    ParseResult parse(char *args[]) {
        int argc = sizeof(args) / sizeof(args[0]);
        return parseContext(argc, args);
    }
};

TEST_F(ContextTest, NoArgs) {
    auto res = parse({});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

TEST_F(ContextTest, WithAllRequiredOptions) {
    char *args[] = {"mqtt", "pub", "-t", "topic", "-a", "127.0.0.1:8883"};
    auto res = parse(args);
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

// TODO more cases
