#include <iostream>
#include <format>

#include "map.hpp"
#include "game.hpp"

auto isNumber(std::string input) { return std::all_of(input.begin(), input.end(), [](const auto& ch){ return std::isdigit(ch); }); }
auto parseArgs(std::vector<std::string>& args) -> std::tuple<size_t, size_t, size_t> {
    auto width = default_width;
    auto height = default_height;
    auto bombs = default_bombs;

    if(args.size() >= 2 && isNumber(args.at(0))) {
        auto input_width = std::stoi(args.at(0));
        if(input_width >= min_width && input_width <= max_width) width = input_width;

        if(isNumber(args.at(1))) {
            auto input_height = std::stoi(args.at(1));
            if(input_height >= min_height && input_height <= max_height) height = input_height;
        }
    }
    if(args.size() >= 3 && isNumber(args.at(2))) {
        auto input_bombs = std::stoi(args.at(2));
        if(input_bombs >= 0 && input_bombs < width*height) bombs = input_bombs;
    }
        
    return {width, height, bombs};
}

auto main(int argc, char** argv) -> int {
    auto args = std::vector<std::string>(argv + 1, argv + argc);
    const auto [width, height, bombs] = parseArgs(args);

    auto game = Game{width, height, bombs};
    game.run();
}
