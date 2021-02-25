#include "engine.hpp"

void Engine::draw()
{
  window_.clear(sf::Color::Black);

  // Draw the walls
  for (auto &w: walls_)
  {
    window_.draw(w.getWall());
  }

  // Draw the apple
  window_.draw(apple_.getApple());

  // Draw snake blocks
  for (auto &s: snake_)
  {
    window_.draw(s.getBlock());
  }

  window_.display();
}
