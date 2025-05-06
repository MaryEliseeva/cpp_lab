#include "header.h"
sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Gem Match");


Game::Game() {
    grid = std::vector<std::vector<Tile>>(GRID_SIZE, std::vector<Tile>(GRID_SIZE));
    srand((time(nullptr)));

    for (int i = 0; i < GRID_SIZE; ++i)
        for (int j = 0; j < GRID_SIZE; ++j)
            grid[i][j].generate(j, i);

    redTexture.loadFromFile("red.png");
    greenTexture.loadFromFile("green.png");
    blueTexture.loadFromFile("blue.png");
    yellowTexture.loadFromFile("yellow.png");
    purpleTexture.loadFromFile("purple.png");
    lilacTexture.loadFromFile("lil.png");
    bonusTexture.loadFromFile("bonus.png");
    tileTextures = { redTexture, greenTexture, blueTexture, yellowTexture, purpleTexture, lilacTexture };


}

void Game::run() {
    while (app.isOpen()) {
        handleInput();
        while (matchFound) {
            matchFound = false;
            for (Bonus& b : activeBonuses) {
                drawField(); 
                sf::sleep(sf::milliseconds(300)); 
                b.apply(grid); 
            }
            activeBonuses.clear();
            drawField();
            processMatches();
            collapseTiles();
            drawField();
        }
    }
}

bool Game::hasMatchAt(int x, int y) {
    int c = grid[y][x].color;
    if (c == -1) return false;

    // Проверка по горизонтали
    int count = 1;
    for (int i = x - 1; i >= 0 && grid[y][i].color == c; --i) ++count;
    for (int i = x + 1; i < GRID_SIZE && grid[y][i].color == c; ++i) ++count;
    if (count >= 3) return true;

    // Проверка по вертикали
    count = 1;
    for (int i = y - 1; i >= 0 && grid[i][x].color == c; --i) ++count;
    for (int i = y + 1; i < GRID_SIZE && grid[i][x].color == c; ++i) ++count;
    if (count >= 3) return true;

    return false;
}

bool Game::causesMatch(sf::Vector2i a, sf::Vector2i b) {

    std::swap(grid[a.y][a.x].color, grid[b.y][b.x].color);
    bool result = hasMatchAt(a.x, a.y) || hasMatchAt(b.x, b.y);
    std::swap(grid[a.y][a.x].color, grid[b.y][b.x].color);
    return result;
}


void Game::handleInput() {
    sf::Event event;
    while (app.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            app.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i clicked = sf::Mouse::getPosition(app);
            clicked.x /= TILE_WIDTH;
            clicked.y /= TILE_HEIGHT;

            if (firstClick.x == -1)
                firstClick = clicked;
            else {
                if (validSwap(firstClick, clicked) && causesMatch(firstClick, clicked)) {
                    std::swap(grid[firstClick.y][firstClick.x].color, grid[clicked.y][clicked.x].color);
                    matchFound = true;
                }
                firstClick = { -1, -1 };
            }
        }
    }
}

bool Game::validSwap(sf::Vector2i a, sf::Vector2i b) {
    int dx = std::abs(a.x - b.x);
    int dy = std::abs(a.y - b.y);
    return (dx + dy == 1);
}


void Game::drawField() {
    app.clear(sf::Color::White);
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j].color == -1) continue;

            tileSprite.setTexture(tileTextures[grid[i][j].color]);
            tileSprite.setColor(sf::Color(0, 0, 0, 100)); 
            tileSprite.setPosition(grid[i][j].x + 5, grid[i][j].y - 2);
            app.draw(tileSprite);

            tileSprite.setColor(sf::Color::White); 
            tileSprite.setPosition(grid[i][j].x, grid[i][j].y);
            app.draw(tileSprite);
        }
    }

    for (const Bonus& b : activeBonuses) {
        bonusSprite.setTexture(bonusTexture);
        bonusSprite.setPosition(b.px, b.py);
        app.draw(bonusSprite);
    }

    app.display();
}
void Game::processMatches() {
    // Горизонтально
    for (int i = 0; i < GRID_SIZE; ++i) {
        int j = 0;
        while (j < GRID_SIZE - 2) {
            int c = grid[i][j].color;
            if (c == -1) {
                ++j;
                continue;
            }

            int length = 1;
            while (j + length < GRID_SIZE && grid[i][j + length].color == c)
                ++length;

            if (length >= 3) {
                matchFound = true;
                for (int k = 0; k < length; ++k)
                    grid[i][j + k].toRemove = true;

                if (length > 3) {
                    int bonusPos = j + length / 2;
                    if (!grid[i][bonusPos].hasBonus) {
                        Bonus b;
                        b.spawn(bonusPos, i, grid);
                        activeBonuses.push_back(b);
                    }
                }

                j += length;
            }
            else {
                ++j;
            }
        }
    }

    // Вертикально
    for (int j = 0; j < GRID_SIZE; ++j) {
        int i = 0;
        while (i < GRID_SIZE - 2) {
            int c = grid[i][j].color;
            if (c == -1) {
                ++i;
                continue;
            }

            int length = 1;
            while (i + length < GRID_SIZE && grid[i + length][j].color == c)
                ++length;

            if (length >= 3) {
                matchFound = true;
                for (int k = 0; k < length; ++k)
                    grid[i + k][j].toRemove = true;

                if (length > 3) {
                    int bonusPos = i + length / 2;
                    if (!grid[bonusPos][j].hasBonus) {
                        Bonus b;
                        b.spawn(j, bonusPos, grid);
                        activeBonuses.push_back(b);
                    }
                }

                i += length;
            }
            else {
                ++i;
            }
        }
    }

    for (std::vector<Tile>& row : grid)
        for (Tile& t : row)
            if (t.toRemove) {
                t.color = -1;
                t.toRemove = false;
                t.hasBonus = false;
            }

}


void Game::collapseTiles() {
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int y = GRID_SIZE - 1; y >= 0; --y) {
            if (grid[y][x].color == -1) {
                for (int k = y - 1; k >= 0; --k) {
                    if (grid[k][x].color != -1) {
                        std::swap(grid[y][x].color, grid[k][x].color);
                        break;
                    }
                }
            }
        }

        for (int y = 0; y < GRID_SIZE; ++y) {
            if (grid[y][x].color == -1)
                grid[y][x].generate(x, y);
        }
    }
}

void Bonus::spawn(int gx, int gy, std::vector<std::vector<Tile>>& grid) {
    type = rand() % 2;
    bonusColor = grid[gy][gx].color;
    px = grid[gy][gx].x;
    py = grid[gy][gx].y;
    grid[gy][gx].hasBonus = true;
}

void Bonus::apply(std::vector<std::vector<Tile>>& grid) {
    int gx = py / TILE_HEIGHT;
    int gy = px / TILE_WIDTH;

    int count = 0;
    while (count < 2) {
        int rx = rand() % GRID_SIZE;
        int ry = rand() % GRID_SIZE;

        if (std::abs(rx - gx) + std::abs(ry - gy) >= 2 && grid[ry][rx].color != -1) {
            grid[ry][rx].color = bonusColor;
            ++count;
        }
    }

    grid[gy][gx].hasBonus = false;
}
