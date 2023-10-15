#pragma once

#include <SFML/Graphics.hpp>
#include "map.hpp"
#include "counter.hpp"

class Game {
public:
    Game(size_t, size_t, size_t);
    
    auto run() -> void;
    auto handleInput(sf::Event) -> void;
    auto check() -> void;
    auto render() -> void;
    auto getStatus() -> GameStatus;

private:
    sf::RenderWindow window;
    Map map;

    Counter bombCounter;
    GameStatus status = NOT_STARTED;
    sf::Texture tileset_texture;
};