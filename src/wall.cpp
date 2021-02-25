#include "wall.hpp"

Wall::Wall(sf::Vector2f position, sf::Vector2f size)
{
  wall_.setSize(size);
  wall_.setFillColor(sf::Color::White);
  wall_.setPosition(position);
}

sf::RectangleShape Wall::getWall() const
{
  return wall_;
}
