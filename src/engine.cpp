#include "engine.hpp"

#include <iostream>

typedef values::Size Size;

const sf::Time Engine::TIME_PER_FRAME = sf::seconds(1.f / 60.f);

Engine::Engine()
{
  // Window
  resolution_ = sf::Vector2f(Size::RESOLUTION_HEIGHT, Size::RESOLUTION_WIDTH);
  window_.create(sf::VideoMode(resolution_.x, resolution_.y), "Snake Game", sf::Style::Default);
  window_.setFramerateLimit(FPS);

  maxLevels_ = 0;
  checkLevelFiles();

  startGame();

  currentGameState_ = GameState::RUNNING;
  lastGameState_ = currentGameState_;
}

void Engine::startGame()
{
  speed_ = 4;
  snakeDirection_ = Direction::RIGHT;
  blocksToAdd_ = 0;

  timeSinceLastMove_ = sf::Time::Zero;

  directionQueue_.clear();
  currentLevel_ = 1;
  loadLevel(currentLevel_);
  newSnake();
  moveApple();

  currentGameState_ = GameState::RUNNING;
  lastGameState_ = currentGameState_;
}

void Engine::pauseGame()
{
  if (currentGameState_ == GameState::RUNNING)
  {
    lastGameState_ = currentGameState_;
    currentGameState_ = GameState::PAUSED;
  }
  else if (currentGameState_ == GameState::PAUSED)
  {
    currentGameState_ = lastGameState_;
  }
}

void Engine::checkLevelFiles()
{
  std::ifstream levelsFile("assets/levels/levels.txt");
  std::ifstream testFile;
  for (std::string levelsFileLine; getline(levelsFile, levelsFileLine);)
  {
    testFile.open("assets/levels/" + levelsFileLine);
    if (testFile.is_open())
    {
      levels_.emplace_back("assets/levels/" + levelsFileLine);
      testFile.close();
      ++maxLevels_;
    }
  }
}

void Engine::loadLevel(int levelNumber)
{
  std::string levelFile = levels_[levelNumber - 1];
  std::ifstream level(levelFile);
  std::string line;
  if (level.is_open())
  {
    for (int y = 0; y < 30; y++)
    {
      getline(level, line);
      for (int x = 0; x < 40; x++)
      {
        if (line[x] == 'w')
        {
          walls_.emplace_back(Wall(sf::Vector2f(x * 20, y * 20), sf::Vector2f(20, 20)));
        }
      }
    }
  }
  level.close();
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
    newApplePosition.x =
        static_cast<float>(1 + rand() / ((RAND_MAX + 1u) / static_cast<int>(appleResolution.x)) * appleSize);
    newApplePosition.y =
        static_cast<float>(1 + rand() / ((RAND_MAX + 1u) / static_cast<int>(appleResolution.y)) * appleSize);

    // Check if it's in the snake
    for (auto &s: snake_)
    {
      if (s.getBlock().getGlobalBounds().intersects(sf::Rect<float>(newApplePosition.x,
                                                                    newApplePosition.y,
                                                                    appleSize,
                                                                    appleSize)))
      {
        badPosition = true;
        break;
      }
    }

    // Check if it's in the walls
    for (auto &w: walls_)
    {
      if (w.getWall().getGlobalBounds().intersects(sf::Rect<float>(newApplePosition.x,
                                                                   newApplePosition.y,
                                                                   Size::WALL,
                                                                   Size::WALL)))
      {
        badPosition = true;
        break;
      }
    }

    // todo: check if it's on wall

  } while (badPosition);

  apple_.setPosition(newApplePosition);
}

void Engine::run()
{
  sf::Clock clock;

  // Main loop - Runs until the window is closed
  while (window_.isOpen())
  {
    sf::Time dt = clock.restart();

    if (currentGameState_ == GameState::PAUSED || currentGameState_ == GameState::GAMEOVER)
    {
      input();

      if (currentGameState_ == GameState::GAMEOVER)
      {
        draw();
      }

      sleep(sf::milliseconds(2));
      continue;
    }

    timeSinceLastMove_ += dt;

    input();
    update();
    draw();
  }
}
