#pragma once

#include <format>
#include <iostream>
#include <algorithm>
#include <random>
#include <SFML/Graphics.hpp>

constexpr auto default_width = 15;
constexpr auto default_height = 15;
constexpr auto min_height = 2;
constexpr auto min_width = 2;
constexpr auto max_width = 120;
constexpr auto max_height = 72;
constexpr auto default_bombs = 54;
constexpr auto tile_size = 32;

enum GameStatus { NOT_STARTED, ONGOING, FAILED, WON };

struct Tile {
    bool hasBomb = false;
    bool uncovered = false;
    bool isFlagged = false;
    int neighbouringBombs = 0;
};

class Map {
public:
    Map(size_t, size_t, size_t);
    auto uncover(size_t, size_t) -> void;
    auto toggleFlag(size_t, size_t) -> bool;
    auto hasBomb(size_t, size_t) -> bool;
    auto isFlagged(size_t, size_t) -> bool;
    auto neighboringBombCount(size_t, size_t) -> int;
    auto isUncovered(size_t, size_t) -> bool;
    auto getBombCount() -> size_t;
    auto getWidth() -> size_t;
    auto getHeight() -> size_t;
    auto isGenerated() -> bool;
    auto generateMap(size_t, size_t) -> void;
    auto draw(sf::RenderWindow&, sf::Texture&, GameStatus) -> void;
    auto getMapVector() -> std::vector<Tile>&;
private:
    auto getTile(size_t, size_t) -> Tile&;
    bool generated = false;
    size_t bombCount;
    size_t width;
    size_t height;
    std::vector<Tile> map;
};