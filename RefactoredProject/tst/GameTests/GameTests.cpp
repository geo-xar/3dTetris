#include "gtest/gtest.h"

class GameTests final
{
public:
};

TEST(GameTests, firstGameTest)
{

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
