#include "engine.hpp"

typedef values::Size Size;

void Engine::update()
{
  // Updating snake position
  if (timeSinceLastMove_.asSeconds() >= sf::seconds(1.f / static_cast<float>(speed_)).asSeconds())
  {
    sf::Vector2f thisBlockPosition = snake_.front().getPosition();
    sf::Vector2f lastBlockPosition = thisBlockPosition;

    if (!directionQueue_.empty())
    {
      switch (snakeDirection_)
      {
        case Direction::UP:
          if (directionQueue_.front() != Direction::DOWN)
          {
            snakeDirection_ = directionQueue_.front();
          }
          break;
        case Direction::RIGHT:
          if (directionQueue_.front() != Direction::LEFT)
          {
            snakeDirection_ = directionQueue_.front();
          }
          break;
        case Direction::DOWN:
          if (directionQueue_.front() != Direction::UP)
          {
            snakeDirection_ = directionQueue_.front();
          }
          break;
        case Direction::LEFT:
          if (directionQueue_.front() != Direction::RIGHT)
          {
            snakeDirection_ = directionQueue_.front();
          }
          break;
      }

      directionQueue_.pop_front();
    }

    // Updating score
    score_ = snake_.size() * (eatenApplesTotal_ + 1);
    scoreText_.setString(std::to_string(score_));
    sf::FloatRect scoreTextBounds = scoreText_.getGlobalBounds();
    scoreText_.setPosition(sf::Vector2f(resolution_.x - scoreTextBounds.width - 15, -6));


    // If we need to grow up the snake
    if (blocksToAdd_ != 0)
    {
      addSnakeBlock();
      --blocksToAdd_;
    }

    // Updating snake's head position
    switch (snakeDirection_)
    {
      case Direction::UP:
        snake_.front().setPosition(sf::Vector2f(thisBlockPosition.x, thisBlockPosition.y - Size::SNAKE_BLOCK_SIZE));
        break;
      case Direction::RIGHT:
        snake_.front().setPosition(sf::Vector2f(thisBlockPosition.x + Size::SNAKE_BLOCK_SIZE, thisBlockPosition.y));
        break;
      case Direction::DOWN:
        snake_.front().setPosition(sf::Vector2f(thisBlockPosition.x, thisBlockPosition.y + Size::SNAKE_BLOCK_SIZE));
        break;
      case Direction::LEFT:
        snake_.front().setPosition(sf::Vector2f(thisBlockPosition.x - Size::SNAKE_BLOCK_SIZE, thisBlockPosition.y));
        break;
    }

    // Updating snake's tail position
    for (int i = 1; i < snake_.size(); ++i)
    {
      thisBlockPosition = snake_[i].getPosition();
      snake_[i].setPosition(lastBlockPosition);
      lastBlockPosition = thisBlockPosition;
    }

    // Updating snake blocks
    for (auto &s: snake_)
    {
      s.update();
    }

    // If snake hits the Apple
    if (snake_.front().getBlock().getGlobalBounds().intersects(apple_.getApple().getGlobalBounds()))
    {
      ++eatenApplesAtLevel_;
      ++eatenApplesTotal_;
      eatenApplesText_.setString("Apples: " + std::to_string(eatenApplesTotal_));
      sf::FloatRect currentLevelTextBounds = currentLevelText_.getGlobalBounds();
      eatenApplesText_.setPosition(sf::Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -6));

      bool newLevel = false;
      if (eatenApplesAtLevel_ >= 5)
      {
        if (currentLevel_ < maxLevels_)
        {
          newLevel = true;
          startNextLevel();
        }
      }

      if (!newLevel)
      {
        blocksToAdd_ += 4;
        ++speed_;
        moveApple();
      }
    }

    // If snake hits itself
    for (int i = 1; i < snake_.size(); ++i)
    {
      if (snake_[0].getBlock().getGlobalBounds().intersects(snake_[i].getBlock().getGlobalBounds()))
      {
        currentGameState_ = GameState::GAMEOVER;
      }
    }

    // If snake hits the walls
    for (auto &w : walls_)
    {
      if (snake_[0].getBlock().getGlobalBounds().intersects(w.getWall().getGlobalBounds()))
      {
        currentGameState_ = GameState::GAMEOVER;
      }
    }

    timeSinceLastMove_ = sf::Time::Zero;
  }
}
