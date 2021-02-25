#include "apple.hpp"

typedef values::Size Size;

Apple::Apple()
{
  sf::Vector2f startPosition(400, 200);

  apple_.setSize(sf::Vector2f(Size::APPLE_SIZE - 2, Size::APPLE_SIZE - 2));
  apple_.setFillColor(sf::Color::Red);
  apple_.setPosition(startPosition);
}

void Apple::setPosition(sf::Vector2f newPosition)
{
  apple_.setPosition(newPosition);
}

sf::RectangleShape Apple::getApple() const
{
  return apple_;
}
