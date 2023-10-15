#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class Counter {
public:
    Counter(size_t digits) : max_digits(digits) {
        if(digits_texture.loadFromFile("res/numbers.png")) { /* err */ }
    }

    auto setNumber(size_t number) -> void { current_number = number; }
    auto increment() -> void { current_number++; }
    auto decrement() -> void { current_number--; }
    auto getNumber() -> size_t { return current_number; }
    auto render(sf::RenderWindow& window, int x, int y) -> void {
        auto tmpNumber = current_number;
        auto digits = (tmpNumber != 0) ? std::floor(std::log10(tmpNumber)) + 1 : 1;
        auto buffer = max_digits - digits;
        auto rect = [](int d){ return sf::IntRect{d * 36, 0, 36, 72}; };
        for(auto i = max_digits; i > 0; i--) {
            auto sprite = sf::Sprite(digits_texture);
            if(i > buffer) {
                sprite.setTextureRect(rect(tmpNumber % 10));
                tmpNumber /= 10;
            } else sprite.setTextureRect(rect(10));
            sprite.setPosition(x + ((i-1) * 36), y);
            window.draw(sprite);
        }
    }

private:
    sf::Texture digits_texture;
    size_t max_digits = 1;
    size_t current_number = 0;
};