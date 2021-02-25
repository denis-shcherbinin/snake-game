#ifndef SNAKE_GAME_HEADERS_ENGINE_HPP_
#define SNAKE_GAME_HEADERS_ENGINE_HPP_

#include <vector>
#include <deque>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "values.hpp"
#include "snake-block.hpp"
#include "apple.hpp"
#include "wall.hpp"

class Engine
{
 private:
  // Window
  sf::Vector2f resolution_;
  sf::RenderWindow window_;
  const unsigned int FPS = 60;

  // Snake
  std::vector<SnakeBlock> snake_;

  int speed_;
  int blocksToAdd_;
  int snakeDirection_;
  std::deque<int> directionQueue_;

  int eatenApplesTotal_;
  int eatenApplesAtLevel_;
  unsigned long long int score_;

  // Apple
  Apple apple_;

  std::vector<Wall> walls_;

  // Levels
  int currentLevel_;
  int maxLevels_;
  std::vector<std::string> levels_;

  sf::Font mainFont_;
  sf::Text titleText_;
  sf::Text currentLevelText_;
  sf::Text eatenApplesText_;
  sf::Text scoreText_;
  sf::Text gameOverText_;
  sf::Text pressEnterText_;
  sf::Text gamePausedText_;
  sf::Text pressPauseText_;

  sf::Time timeSinceLastMove_;

  int currentGameState_;
  int lastGameState_;
 public:
  enum Direction
  {
    UP,
    RIGHT,
    DOWN,
    LEFT,
  };

  enum GameState
  {
    RUNNING,
    PAUSED,
    GAMEOVER,
  };

  Engine();

  static void setupText(sf::Text *textItem, const sf::Font &font, const sf::String &value, int size, sf::Color color);

  void startGame();
  void pauseGame();

  void input();
  void update();
  void draw();

  void newSnake();
  void addSnakeBlock();

  void addDirection(int newDirection);

  void moveApple();
  void checkLevelFiles();
  void loadLevel(int levelNumber);

  void startNextLevel();

  void run();
};

#endif //SNAKE_GAME_HEADERS_ENGINE_HPP_
