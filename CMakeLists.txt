cmake_minimum_required(VERSION 3.17)
project(snake-game)

set(CMAKE_CXX_STANDARD 20)

add_executable(snake-game src/main.cpp headers/snake-block.hpp src/snake-block.cpp headers/engine.hpp src/engine.cpp src/input.cpp src/draw.cpp headers/apple.hpp src/apple.cpp headers/values.hpp src/update.cpp headers/wall.hpp src/wall.cpp)

include_directories(headers /usr/include C:/SFML/include)

set(SFML_ROOT c:/SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake-modules")
find_package(SFML REQUIRED system window graphics)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(snake-game ${SFML_LIBRARIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build directory
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()
