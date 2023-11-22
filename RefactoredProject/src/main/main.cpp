#include "Game.h"
#include "GridFactory.h"
#include "LoggerFactory.h"

int main(int argc, char** argv)
{
    auto logger = LoggerFactory::createLogger();
    auto grid = GridFactory::createGrid();
    Game game{*logger, *grid};
    return 0;
}
