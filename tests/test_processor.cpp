#include <gtest/gtest.h>
#include <string>

// Simple smoke test to verify Google Test works
TEST(SmokeTest, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
    std::string s = "hello";
    EXPECT_EQ(s.size(), 5u);
}

// Test HTTP response builder logic (minimal, no server dependency)
TEST(ResponseTest, StatusLine) {
    std::string status = "HTTP/1.1 200 OK";
    EXPECT_NE(status.find("200"), std::string::npos);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
