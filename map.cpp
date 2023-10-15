#include "map.hpp"

Map::Map(size_t _width, size_t _height, size_t _bombs)
    : bombCount(_bombs), width(_width), height(_height)
{
    
}

auto Map::generateMap(size_t startX, size_t startY) -> void {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<size_t> dist_w(0, width-1);
    std::uniform_int_distribution<size_t> dist_h(0, height-1);

    map = std::vector<Tile>{width*height, Tile{false, false, false, 0}};
    for(auto i = 0u; i < bombCount; i++) {
        auto x = startX; 
        auto y = startY;
        while(x == startX && y == startY) {
            x = dist_w(mt);
            y = dist_h(mt);
        }

        getTile(x,y).hasBomb = true;
    }

    auto countBombs = [&](size_t x, size_t y) {
        auto count = 0u;
        if(x > 0 && y > 0) if(getTile(x-1, y-1).hasBomb) count++;
        if(y > 0) if(getTile(x, y-1).hasBomb) count++;
        if(x < width-1 &&  y > 0) if(getTile(x+1, y-1).hasBomb) count++;
        if(x > 0) if(getTile(x-1, y).hasBomb) count++;
        if(x < width-1) if(getTile(x + 1, y).hasBomb) count++;
        if(x > 0 &&  y < height-1) if(getTile(x-1, y+1).hasBomb) count++;
        if(y < height-1) if(getTile(x, y+1).hasBomb) count++;
        if(x < width-1 && y < height-1) if(getTile(x+1, y+1).hasBomb) count++;
        return count;
    }; 

    for(auto x = 0u; x < width; x++) {
        for(auto y = 0u; y < height; y++) {
            getTile(x,y).neighbouringBombs = countBombs(x,y);
        }
    }

    generated = true;
}

auto Map::uncover(size_t x, size_t y) -> void {
    getTile(x,y).uncovered = true;

    if(!hasBomb(x,y) && neighboringBombCount(x,y) == 0) {
        if(x > 0 && y > 0)              if(!getTile(x-1, y-1).hasBomb && !isUncovered(x-1, y-1)) uncover(x-1, y-1);
        if(y > 0)                       if(!getTile(x,   y-1).hasBomb && !isUncovered(x  , y-1)) uncover(x,   y-1);
        if(x < width-1 && y > 0)        if(!getTile(x+1, y-1).hasBomb && !isUncovered(x+1, y-1)) uncover(x+1, y-1);
        if(x > 0)                       if(!getTile(x-1, y  ).hasBomb && !isUncovered(x-1, y  )) uncover(x-1, y);
        if(x < width-1)                 if(!getTile(x+1, y  ).hasBomb && !isUncovered(x+1, y  )) uncover(x+1, y);
        if(x > 0 && y < height-1)       if(!getTile(x-1, y+1).hasBomb && !isUncovered(x-1, y+1)) uncover(x-1, y+1);
        if(y < height-1)                if(!getTile(x,   y+1).hasBomb && !isUncovered(x,   y+1)) uncover(x,   y+1);
        if(x < width-1 && y < height-1) if(!getTile(x+1, y+1).hasBomb && !isUncovered(x+1, y+1)) uncover(x+1, y+1);
    }
}

auto Map::draw(sf::RenderWindow& window, sf::Texture& tileset, GameStatus status) -> void {
    for(auto x = 0u; x < width; x++) {
        for(auto y = 0u; y < height; y++) {     
            auto rect = [](int _x, int _y){ return sf::IntRect(_x * tile_size, _y * tile_size, tile_size, tile_size); };
            auto tileSprite = sf::Sprite(tileset);
            tileSprite.setPosition(x * tile_size, y * tile_size);
            if(!isGenerated()) {
                tileSprite.setTextureRect(rect(0, 1));
            } else if(status == GameStatus::FAILED) {
                if(!isUncovered(x,y) && hasBomb(x,y)) tileSprite.setTextureRect(rect(4,1));
                else if(!isFlagged(x,y) && hasBomb(x,y)) tileSprite.setTextureRect(rect(3,1));
                else if(isFlagged(x,y) && hasBomb(x,y)) tileSprite.setTextureRect(rect(1,1));
                else if(isFlagged(x,y) && !hasBomb(x,y)) tileSprite.setTextureRect(rect(2,1));
                else if(!hasBomb(x,y)) tileSprite.setTextureRect(rect(neighboringBombCount(x,y), 0));
            } else {
                if(isUncovered(x,y)) tileSprite.setTextureRect(rect(
                    hasBomb(x,y) ? 3 : neighboringBombCount(x,y),
                    hasBomb(x,y) ? 1 : 0
                )); 
                else tileSprite.setTextureRect(rect(
                    isFlagged(x,y) ? 1 : 0,
                    1
                ));
            }

            window.draw(tileSprite);
        }
    }
}

auto Map::getMapVector() -> std::vector<Tile>& { return map; }
auto Map::getTile(size_t x, size_t y) -> Tile& { return map.at(x * height + y); }
auto Map::isFlagged(size_t x, size_t y) -> bool { return getTile(x,y).isFlagged; }
auto Map::neighboringBombCount(size_t x, size_t y) -> int { return getTile(x,y).neighbouringBombs; }
auto Map::toggleFlag(size_t x, size_t y) -> bool { 
    getTile(x,y).isFlagged = !getTile(x,y).isFlagged; 
    return getTile(x,y).isFlagged;
}
auto Map::hasBomb(size_t x, size_t y) -> bool { return getTile(x,y).hasBomb; }
auto Map::isUncovered(size_t x, size_t y) -> bool { return getTile(x,y).uncovered; }
auto Map::getWidth() -> size_t { return width; }
auto Map::getHeight() -> size_t { return height; }
auto Map::getBombCount() -> size_t { return bombCount; }
auto Map::isGenerated() -> bool { return generated; }
