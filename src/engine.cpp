#include "engine.hpp"

typedef values::Size Size;

const sf::Time Engine::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

Engine::Engine()
{
  // Window
  resolution_ = sf::Vector2f(Size::RESOLUTION_HEIGHT, Size::RESOLUTION_WIDTH);
  window_.create(sf::VideoMode(resolution_.x, resolution_.y), "Snake Game", sf::Style::Default);
  window_.setFramerateLimit(FPS);

  // Speed and direction
  speed_ = 6;
  snakeDirection_ = Direction::RIGHT;

  // Time
  timeSinceLastMove_ = sf::Time::Zero;

  blocksToAdd_ = 0;

  newSnake();
  moveApple();
}

void Engine::newSnake()
{
  snake_.clear();
  snake_.emplace_back(sf::Vector2f(200, 100));
  snake_.emplace_back(sf::Vector2f(180, 100));
  snake_.emplace_back(sf::Vector2f(160, 100));
}

void Engine::addSnakeBlock()
{
  sf::Vector2f newBlockPosition = snake_.back().getPosition();
  snake_.emplace_back(newBlockPosition);
}

void Engine::moveApple()
{
  // Find a position to place the apple(not a snake or wall)
  int appleSize = Size::APPLE_SIZE;

  // Divide the field into blocks the size of apple - remove 2 to exclude the exterior walls
  sf::Vector2f appleResolution = sf::Vector2f(resolution_.x / appleSize - 2, resolution_.y / appleSize - 2);

  sf::Vector2f newApplePosition;
  bool badPosition = false;
  srand(time(nullptr));

  // Looking for a valid position
  do
  {
    badPosition = false;

    // Random position
    newApplePosition.x = static_cast<float>(1 + rand() / ((RAND_MAX + 1u) / static_cast<int>(appleResolution.x)) * appleSize);
    newApplePosition.y = static_cast<float>(1 + rand() / ((RAND_MAX + 1u) / static_cast<int>(appleResolution.y)) * appleSize);

    // Check if it's in the snake
    for (auto &s: snake_)
    {
      if (s.getBlock().getGlobalBounds().intersects(sf::Rect<float>(newApplePosition.x, newApplePosition.y, appleSize, appleSize)))
      {
        badPosition = true;
        break;
      }
    }

    /// todo: Check if it's in the wall

  } while (badPosition);

  apple_.setPosition(newApplePosition);
}

void Engine::run()
{
  sf::Clock clock;

  while (window_.isOpen())
  {
    sf::Time dt = clock.restart();
    timeSinceLastMove_ += dt;

    input();
    update();
    draw();
  }
}
