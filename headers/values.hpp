#ifndef SNAKE_GAME_HEADERS_VALUES_HPP_
#define SNAKE_GAME_HEADERS_VALUES_HPP_

namespace values
{
  enum Size
  {
    SNAKE_BLOCK_SIZE = 20,
    APPLE_SIZE = SNAKE_BLOCK_SIZE,
    RESOLUTION_HEIGHT = 800,
    RESOLUTION_WIDTH = 600,
    WALL = 20,
  };

  enum Limits
  {
    APPLES_TO_EAT = 5,
  };
}

#endif //SNAKE_GAME_HEADERS_VALUES_HPP_
