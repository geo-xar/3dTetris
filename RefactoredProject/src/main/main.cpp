#include <LoggerFactory.h>
#include <Game.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    auto logger = LoggerFactory::createLogger();
    return 0;
}
