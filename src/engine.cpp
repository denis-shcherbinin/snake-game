#include "engine.hpp"

typedef values::Size Size;

void Engine::setupText(sf::Text *textItem, const sf::Font &font, const sf::String &value, int size, sf::Color color)
{
  textItem->setFont(font);
  textItem->setString(value);
  textItem->setCharacterSize(size);
  textItem->setFillColor(color);
}

Engine::Engine()
{
  resolution_ = sf::Vector2f(Size::RESOLUTION_HEIGHT, Size::RESOLUTION_WIDTH);
  window_.create(sf::VideoMode(resolution_.x, resolution_.y), "Snake Game", sf::Style::Default);
  window_.setFramerateLimit(FPS);

  maxLevels_ = 0;
  checkLevelFiles();

  startGame();

  mainFont_.loadFromFile("../assets/fonts/SF-UI-Display-Regular.otf");

  setupText(&titleText_, mainFont_, "Snake Game", 24, sf::Color::Magenta);
  sf::FloatRect titleTextBounds = titleText_.getGlobalBounds();
  titleText_.setPosition(sf::Vector2f(resolution_.x / 2 - titleTextBounds.width / 2, -6));

  setupText(&currentLevelText_, mainFont_, "LvL 1", 24, sf::Color::Magenta);
  currentLevelText_.setPosition(sf::Vector2f(15, -6));
  sf::FloatRect currentLevelTextBounds = currentLevelText_.getGlobalBounds();

  setupText(&eatenApplesText_, mainFont_, "Apples: 0", 24, sf::Color::Magenta);
  eatenApplesText_.setPosition(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -6);

  setupText(&scoreText_, mainFont_, std::to_string(score_), 24, sf::Color::Magenta);
  sf::FloatRect scoreTextBounds = scoreText_.getGlobalBounds();
  scoreText_.setPosition(sf::Vector2f(resolution_.x - scoreTextBounds.width - 15, -6));

  setupText(&gameOverText_, mainFont_, "GAME OVER", 72, sf::Color::Yellow);
  sf::FloatRect gameOverTextBounds = gameOverText_.getLocalBounds();
  gameOverText_.setPosition(sf::Vector2f(resolution_.x / 2 - gameOverTextBounds.width / 2, 100));
  gameOverText_.setOutlineColor(sf::Color::Black);
  gameOverText_.setOutlineThickness(2);

  setupText(&pressEnterText_, mainFont_, "Press \"ENTER\" to try again.", 36, sf::Color::Green);
  sf::FloatRect pressEnterTextBounds = pressEnterText_.getLocalBounds();
  pressEnterText_.setPosition(sf::Vector2f(resolution_.x / 2 - pressEnterTextBounds.width / 2, 200));
  pressEnterText_.setOutlineColor(sf::Color::Black);
  pressEnterText_.setOutlineThickness(2);

  setupText(&gamePausedText_, mainFont_, "PAUSE", 72, sf::Color::Yellow);
  sf::FloatRect gamePausedTextBounds = gamePausedText_.getLocalBounds();
  gamePausedText_.setPosition(sf::Vector2f(resolution_.x / 2 - gamePausedTextBounds.width / 2, 100));
  gamePausedText_.setOutlineColor(sf::Color::Black);
  gamePausedText_.setOutlineThickness(2);

  setupText(&pressPauseText_, mainFont_, "Press \"P\" to unpause.", 36, sf::Color::Green);
  sf::FloatRect pressPauseTextBounds = pressPauseText_.getLocalBounds();
  pressPauseText_.setPosition(sf::Vector2f(resolution_.x / 2 - pressPauseTextBounds.width / 2, 200));
  pressPauseText_.setOutlineColor(sf::Color::Black);
  pressPauseText_.setOutlineThickness(2);
}

void Engine::startGame()
{
  score_ = 0;
  speed_ = 4;
  snakeDirection_ = Direction::RIGHT;
  blocksToAdd_ = 0;

  timeSinceLastMove_ = sf::Time::Zero;

  directionQueue_.clear();
  walls_.clear();

  eatenApplesTotal_ = 0;
  eatenApplesAtLevel_ = 0;

  currentLevel_ = 1;
  loadLevel(currentLevel_);
  newSnake();
  moveApple();

  currentGameState_ = GameState::RUNNING;
  lastGameState_ = currentGameState_;

  currentLevelText_.setString("LvL: " + std::to_string(currentLevel_));

  eatenApplesText_.setString("Apples: " + std::to_string(eatenApplesTotal_));
  sf::FloatRect currentLevelTextBounds = currentLevelText_.getGlobalBounds();
  eatenApplesText_.setPosition(sf::Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -6));

  scoreText_.setString(std::to_string(score_));
  sf::FloatRect scoreTextBounds = scoreText_.getLocalBounds();
  scoreText_.setPosition(sf::Vector2f(resolution_.x - scoreTextBounds.width - 15, -6));
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
  std::ifstream levelsFile("../assets/levels/levels.txt");
  std::ifstream testFile;
  for (std::string levelsFileLine; getline(levelsFile, levelsFileLine);)
  {
    testFile.open("../assets/levels/" + levelsFileLine);
    if (testFile.is_open())
    {
      levels_.emplace_back("../assets/levels/" + levelsFileLine);
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
    for (int y = 0; y < static_cast<int>(resolution_.y / Size::WALL); y++)
    {
      getline(level, line);
      for (int x = 0; x < static_cast<int>(resolution_.x / Size::WALL); x++)
      {
        if (line[x] == 'w')
        {
          walls_.emplace_back(Wall(sf::Vector2f(x * Size::WALL, y * Size::WALL), sf::Vector2f(Size::WALL, Size::WALL)));
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
  sf::Vector2f appleResolution = sf::Vector2f(resolution_.x / static_cast<float>(appleSize) - 2,
                                              resolution_.y / static_cast<float>(appleSize) - 2);

  sf::Vector2f newApplePosition;
  bool badPosition;
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

void Engine::startNextLevel()
{
  ++currentLevel_;
  walls_.clear();
  directionQueue_.clear();
  speed_ = 4 + currentLevel_;
  snakeDirection_ = Direction::RIGHT;
  blocksToAdd_ = 0;
  eatenApplesAtLevel_ = 0;

  loadLevel(currentLevel_);
  newSnake();
  moveApple();

  currentLevelText_.setString("LvL: " + std::to_string(currentLevel_));
  sf::FloatRect currentLevelTextBounds = currentLevelText_.getGlobalBounds();
  eatenApplesText_.setPosition(sf::Vector2f(currentLevelTextBounds.left + currentLevelTextBounds.width + 20, -5));
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
