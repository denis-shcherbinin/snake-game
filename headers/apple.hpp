#ifndef SNAKE_GAME_HEADERS_APPLE_HPP_
#define SNAKE_GAME_HEADERS_APPLE_HPP_

#include <SFML/Graphics.hpp>
#include "values.hpp"

class Apple
{
 private:
  sf::RectangleShape apple_;
 public:
  Apple();

  void setPosition(sf::Vector2f newPosition);

  sf::RectangleShape getApple() const;
};

#endif //SNAKE_GAME_HEADERS_APPLE_HPP_
