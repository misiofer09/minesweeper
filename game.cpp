#include "game.hpp"

Game::Game(size_t width, size_t height, size_t bombs) 
    : window(sf::VideoMode(width * tile_size, height * tile_size + 72), "Minesweeper Clone"),
      map(width, height, bombs),
      bombCounter(3)
{
    if(tileset_texture.loadFromFile("res/tiles.png")) { /* err */ }
    bombCounter.setNumber(bombs);
}

auto Game::run() -> void {
    while(window.isOpen()) {
        auto event = sf::Event{};
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::MouseButtonPressed) {
                handleInput(event);
            }
        }
        
        if(status == GameStatus::ONGOING) check();
        render();
    }
}

auto Game::check() -> void {
    auto bombExposed = std::any_of(map.getMapVector().begin(), map.getMapVector().end(), 
        [](const Tile& tile) { return tile.hasBomb && tile.uncovered; });
    auto gameFinished = std::all_of(map.getMapVector().begin(), map.getMapVector().end(), 
        [](const Tile& tile) { return tile.uncovered || tile.isFlagged; });
    size_t flaggedCount = std::count_if(map.getMapVector().begin(), map.getMapVector().end(), 
        [](const Tile& tile) { return tile.isFlagged; });

    if(bombExposed) { 
        status = GameStatus::FAILED;
        for(auto& t : map.getMapVector()) {
            if(t.hasBomb && t.uncovered) { t.uncovered = false; }
            else t.uncovered = true;
        }
    } else if(gameFinished && !bombExposed && flaggedCount == map.getBombCount()) {
        status = GameStatus::WON;
    }
}

auto Game::handleInput(sf::Event event) -> void {
    auto mousePos = sf::Mouse::getPosition(window);
    auto tileX = std::min(static_cast<int>(map.getWidth()-1), std::max(0, mousePos.x/tile_size));
    auto tileY = std::min(static_cast<int>(map.getHeight()-1), std::max(0, mousePos.y/tile_size));            

    if(event.mouseButton.button == sf::Mouse::Button::Left) {
        if(!map.isGenerated()) {
            status = GameStatus::ONGOING;
            map.generateMap(tileX, tileY);
            map.uncover(tileX, tileY);
        } else {
            if(!map.isUncovered(tileX, tileY) && !map.isFlagged(tileX, tileY)) map.uncover(tileX, tileY);
        }
    } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
        if(map.isGenerated()) {
            if(!map.isUncovered(tileX, tileY)) {
                if(map.toggleFlag(tileX, tileY)) bombCounter.decrement();
                else bombCounter.increment();
            }
        }
    }
}

auto Game::render() -> void {
    window.clear(sf::Color::Black);
    map.draw(window, tileset_texture, status);
    bombCounter.render(window, 0, window.getSize().y - 72);
    window.display();
}
