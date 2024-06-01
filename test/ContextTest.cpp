
#include "Context.h"
#include <gtest/gtest.h>

using namespace MqttClient;

TEST(ContextTest, NoArgs) {
    auto res = parseContext(0, {});
    EXPECT_EQ(ParseResultCode::FAILURE, res.code);
}

// TODO more cases
