#include "Game.h"

Game::Game(ILogger& logger, IGrid& grid)
: _state{State::STOPPED}
, _logger{logger}
, _grid{grid}
{
}

void Game::start()
{

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
