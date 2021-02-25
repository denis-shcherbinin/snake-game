#include "engine.hpp"

void Engine::input()
{
  sf::Event event = {};

  while(window_.pollEvent(event))
  {
    switch (event.type)
    {
      case(sf::Event::Closed):
      {
        window_.close();
        break;
      }
      case(sf::Event::KeyPressed):
      {
        // Quit
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
          window_.close();
        }

        // Pause
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
          pauseGame();
        }

        if (currentGameState_ == GameState::GAMEOVER)
        {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
          {
            startGame();
          }
        }
        break;
      }
    }
  }

  // Directions
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    addDirection(Direction::UP);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    addDirection(Direction::RIGHT);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    addDirection(Direction::DOWN);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    addDirection(Direction::LEFT);
  }
}

void Engine::addDirection(int newDirection)
{
  if (directionQueue_.empty())
  {
    directionQueue_.emplace_back(newDirection);
  }
  else
  {
    if (directionQueue_.back() != newDirection)
    {
      directionQueue_.emplace_back(newDirection);
    }
  }
}
