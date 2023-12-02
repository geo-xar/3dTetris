#include "Game.h"

Game::Game(ILogger& logger, IGrid& grid, IMainWindow& mainWindow)
: _state{State::STOPPED}
, _logger{logger}
, _grid{grid}
, _mainWindow{mainWindow}
{
}

void Game::start()
{
    _mainWindow.draw();
}

void Game::stop()
{

}

void Game::pause()
{

}

void Game::showHelp()
{

}

void Game::closeHelp()
{

}

void Game::moveCamera()
{

}

void Game::switchCamera()
{

}
