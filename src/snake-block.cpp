#include "snake-block.hpp"

typedef values::Size Size;

SnakeBlock::SnakeBlock(sf::Vector2f startPosition): position_(startPosition)
{
  block_.setSize(sf::Vector2f(Size::SNAKE_BLOCK_SIZE, Size::SNAKE_BLOCK_SIZE));
  block_.setFillColor(sf::Color::Green);
  block_.setPosition(startPosition);
}

void SnakeBlock::setPosition(sf::Vector2f position)
{
  position_ = position;
}

sf::Vector2f SnakeBlock::getPosition() const
{
  return position_;
}

sf::RectangleShape SnakeBlock::getBlock() const
{
  return block_;
}

void SnakeBlock::update()
{
  block_.setPosition(position_);
}
