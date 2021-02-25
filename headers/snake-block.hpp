#ifndef SNAKE_GAME_HEADERS_SNAKE_BLOCK_HPP_
#define SNAKE_GAME_HEADERS_SNAKE_BLOCK_HPP_

#include <SFML/Graphics.hpp>
#include "values.hpp"

class SnakeBlock
{
 private:
  sf::Vector2f position_;
  sf::RectangleShape block_;
 public:
  explicit SnakeBlock(sf::Vector2f startPosition);

  void setPosition(sf::Vector2f position);

  sf::Vector2f getPosition() const;
  sf::RectangleShape getBlock() const;

  void update();
};

#endif //SNAKE_GAME_HEADERS_SNAKE_BLOCK_HPP_
