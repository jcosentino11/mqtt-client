
#include "Context.h"
#include <gtest/gtest.h>

using namespace MqttClient::Context;

TEST(ContextTest, NoArgs) {
    auto result = parse(0, {});
    EXPECT_EQ(ParseResultStatus::FAILURE, result.status);
}

// TODO more cases
