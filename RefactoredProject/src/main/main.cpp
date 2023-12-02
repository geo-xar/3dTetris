#include "Game.h"
#include "GridFactory.h"
#include "LoggerFactory.h"
#include "MainWindowFactory.h"

int main(int argc, char** argv)
{
    auto logger = LoggerFactory::createLogger();
    auto grid = GridFactory::createGrid();
    auto mainWindow = MainWindowFactory::createMainWindow(argc, argv);
    Game game{*logger, *grid, *mainWindow};
    game.start();
    return 0;
}
