#ifndef SNAKE_GAME_HEADERS_WALL_HPP_
#define SNAKE_GAME_HEADERS_WALL_HPP_

#include <SFML/Graphics.hpp>

class Wall
{
 private:
  sf::RectangleShape wall_;
 public:
  Wall(sf::Vector2f position, sf::Vector2f size);

  sf::RectangleShape getWall() const;
};

#endif //SNAKE_GAME_HEADERS_WALL_HPP_
