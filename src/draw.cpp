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

  // Draw text
  window_.draw(titleText_);
  window_.draw(currentLevelText_);
  window_.draw(eatenApplesText_);
  window_.draw(scoreText_);

  if (currentGameState_ == GameState::PAUSED)
  {
    window_.draw(gamePausedText_);
    window_.draw(pressPauseText_);
  }

  if (currentGameState_ == GameState::GAMEOVER)
  {
    window_.draw(gameOverText_);
    window_.draw(pressEnterText_);
  }

  window_.display();
}
