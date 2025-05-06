#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

constexpr int GRID_SIZE = 12;
constexpr int TILE_WIDTH = 60;
constexpr int TILE_HEIGHT = 60;
constexpr float WINDOW_WIDTH = 730;
constexpr float WINDOW_HEIGHT = 730;
constexpr int BONUS_RADIUS = 3;

struct Tile {
    int color = -1;
    bool toRemove = false;
    bool hasBonus = false;
    int x = 0, y = 0;

    void generate(int gx, int gy) {
        color = rand() % 6;
        toRemove = false;
        hasBonus = false;
        x = gx * TILE_WIDTH;
        y = gy * TILE_HEIGHT;
    }
};

class Bonus {
public:
    int type; 
    int bonusColor;
    int px, py;
    void apply(std::vector<std::vector<Tile>>& grid);
    void spawn(int x, int y, std::vector<std::vector<Tile>>& grid);
};

class Game {
public:
    Game();
    void run();

private:

    std::vector<std::vector<Tile>> grid;
    std::vector<Bonus> activeBonuses;
    bool causesMatch(sf::Vector2i a, sf::Vector2i b);

    sf::Texture redTexture, greenTexture, blueTexture, yellowTexture, purpleTexture, lilacTexture;
    std::vector<sf::Texture> tileTextures;

    sf::Sprite tileSprite;

    sf::Texture bonusTexture;

    sf::Sprite bonusSprite;

    sf::Vector2i firstClick{-1, -1};
    bool matchFound = true;
    bool hasMatchAt(int x, int y);
    void handleInput();
    void drawField();
    bool validSwap(sf::Vector2i a, sf::Vector2i b);
    void processMatches();
    void collapseTiles();
};
