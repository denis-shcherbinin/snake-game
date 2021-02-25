#ifndef SNAKE_GAME_HEADERS_ENGINE_HPP_
#define SNAKE_GAME_HEADERS_ENGINE_HPP_

#include <vector>
#include <deque>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "values.hpp"
#include "snake-block.hpp"
#include "apple.hpp"

class Engine
{
 private:
  // Window
  sf::Vector2f resolution_;
  sf::RenderWindow window_;
  const unsigned int FPS = 60;
  static const sf::Time TIME_PER_FRAME;

  // Snake
  std::vector<SnakeBlock> snake_;

  int speed_;
  int blocksToAdd_;
  int snakeDirection_;
  std::deque<int> directionQueue_;

  // Apple
  Apple apple_;

  sf::Time timeSinceLastMove_;

 public:
  enum Direction
  {
    UP,
    RIGHT,
    DOWN,
    LEFT,
  };

  Engine();

  void input();
  void update();
  void draw();

  void newSnake();
  void addSnakeBlock();

  void addDirection(int newDirection);

  void moveApple();

  void run();
};

#endif //SNAKE_GAME_HEADERS_ENGINE_HPP_
