#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>

constexpr int BLOCK_COUNT_WIDTH = 10;
constexpr int BLOCK_COUNT_HEIGHT = 3;
constexpr float WINDOW_WIDTH = 1030;
constexpr float WINDOW_HEIGHT = 730;
constexpr float BLOCK_HEIGHT = 40.0f;
constexpr float VOZM = M_PI / 180.0;
constexpr float BALL_RADIUS = 15.0f;
constexpr float BLOCK_WIDTH = WINDOW_WIDTH / BLOCK_COUNT_WIDTH;
constexpr float MAX_SPEED = 1.5f;
constexpr int LIFE_BONUS_4 = 2;
constexpr int BLOCK_COUNT_TYPES = 4;
constexpr int START_ANGLE = 60;
constexpr int BONUS_COUNT_TYPES = 5;
constexpr float BONUS_RADIUS = BALL_RADIUS * 0.25;
constexpr float START_SPEED = 0.5f;

class Game;

class Block {
public:
    Block() {};
    Block(int i, int j);
    sf::RectangleShape block;
    float x_pos, y_pos;
    int type;
    int life;
    virtual void onHit(Game& game) = 0;
    virtual ~Block() {};
};

class BlockUnbreakable : public Block {
public:
    BlockUnbreakable(int i, int j) : Block(i, j) {
        type = 1;
        life = -1; // Неразрушимый
        block.setFillColor(sf::Color::Blue);
    }
    void onHit(Game& game) override {};
};

class BlockWithBonus : public Block {
public:
    int bonusType;
    BlockWithBonus(int i, int j, int bonus) : Block(i, j), bonusType(bonus) {
        type = 2;
        life = 1;
        block.setFillColor(sf::Color::Green);
    }
    void onHit(Game& game) override;
};

class BlockSpeedUp : public Block {
public:
    float delta_v;
    BlockSpeedUp(int i, int j, float delta) : Block(i, j), delta_v(delta) {
        type = 3;
        life = 1;
        block.setFillColor(sf::Color::Red);
    }
    void onHit(Game& game) override;
};


class BlockWithHealth : public Block {
public:
    BlockWithHealth(int i, int j) : Block(i, j) {
        type = 4;
        life = LIFE_BONUS_4;
        block.setFillColor(sf::Color::Yellow);
    }
    void onHit(Game& game) override;
};


class Platform {
public:
    Platform();
    void move(float x_cursor);
    sf::RectangleShape block;
    float x_pos, y_pos;
    float speed = START_SPEED;
    float platformWidth;
    ~Platform() {};
};


class Ball {
public:
    Ball();
    float x_pos, y_pos;
    float angle;
    float speed = START_SPEED;
    bool randomRedirectActive = false;
    bool isStuckToPlatform = false; //индикатор прилипания
    sf::CircleShape ball;
    sf::Clock randomRedirectClock;//точка отсчёта для смены направления
    sf::Clock stickClock;      // точка отсчёта времени прилипания
    sf::Time randomRedirectTime; // Время, когда произойдёт смена направления
    virtual void move(Platform& plat, int& Score_Fail);

    virtual ~Ball() {};
private:
    const sf::Time stickDuration = sf::seconds(4);
    void verticalSides_Touch();
    void Platform_Touch(bool& act, Platform& plat, int& Score_Fail);
    void step();
    void RedirectAct();
    void stuckAct(Platform& plat);
    void bottomAct();
    void loss(float gran_bottom, int& Score_Fail);
    void gorizont_Touch(float block_top, float gran_bottom, float gran_left_plat, float gran_right_plat);
};


class Bonus {
public:
    sf::CircleShape ball;
    float x_pos, y_pos;
    bool remote = false;
    int type;
    float speed = START_SPEED;
    Bonus(float x, float y);
    virtual void apply(Game& game) = 0;
    virtual void move();
    virtual ~Bonus() {}
};

// Скорость шарика
class BonusSpeed : public Bonus {
public:
    BonusSpeed(float x, float y) : Bonus(x, y) { type = 1; }
    void apply(Game& game) override;
};

// Изменение размера платформы
class BonusResizePlatform : public Bonus {
public:
    BonusResizePlatform(float x, float y) : Bonus(x, y) { type = 2; }
    void apply(Game& game) override;
};

// Прилипание шарика
class BonusStick : public Bonus {
public:
    BonusStick(float x, float y) : Bonus(x, y) { type = 3; }
    void apply(Game& game) override;
};

// Второе дно
class BonusSecondLife : public Bonus {
public:
    BonusSecondLife(float x, float y) : Bonus(x, y) { type = 4; }
    void apply(Game& game) override;
};

// Случайное изменение направления
class BonusRedirect : public Bonus {
public:
    BonusRedirect(float x, float y) : Bonus(x, y) { type = 5; }
    void apply(Game& game) override;
};


bool bottomPlatformActive = false;
sf::Clock bottomClock;//точка отсчёта времени 2-го дна
const sf::Time bottomDuration = sf::seconds(3);


class Game {
public:
    Game();
    void run();
    int Score_Fail;
    int Score_Win;
    Platform plat;
    Ball ball;
    std::vector<std::vector<Block*>> blocks;
    std::vector<Bonus*> bonuses;
    ~Game();
private:
    void drawField();
    void createBlocks();
    void createPlatform();
    void IsTouchBallBlock();
    void checkBonusCatch();
    void ActiveBonus(Bonus* bonus);
    void UpdateBlocks();
};
