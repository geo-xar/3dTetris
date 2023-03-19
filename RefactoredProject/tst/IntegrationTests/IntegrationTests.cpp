#include "gtest/gtest.h"

class IntegrationTests final
{
public:
};

TEST(IntegrationTests, firstIntegrationTest)
{

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
