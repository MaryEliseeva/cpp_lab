#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>

constexpr int BLOCK_COUNT_WIDTH = 6;
constexpr int BLOCK_COUNT_HEIGHT = 4;
constexpr float WINDOW_WIDTH = 730;
constexpr float WINDOW_HEIGHT = 730;
constexpr float BLOCK_HEIGHT = 40.0f;
constexpr float VOZM = M_PI / 180.0;
constexpr float BALL_RADIUS = 15.0f;
constexpr float BLOCK_WIDTH = WINDOW_WIDTH / BLOCK_COUNT_WIDTH;
constexpr float MAX_SPEED = 1.0f;
constexpr int LIFE_BONUS_4 = 2;
constexpr int BLOCK_COUNT_TYPES = 4;
constexpr int START_ANGLE = 60;
constexpr int BONUS_COUNT_TYPES = 5;
constexpr float BONUS_RADIUS = BALL_RADIUS * 0.25;

class Block {
public:
    Block() {};
    Block(int i, int j);
    sf::RectangleShape block;
    float x_pos, y_pos;
    int type;
    int life;
    ~Block() {};
};

class Platform {
public:
    Platform();
    void move(float x_cursor);
    sf::RectangleShape block;
    float x_pos, y_pos;
    float speed = 0.2f;
    float platformWidth;
    ~Platform() {};
};


class Ball {
public:
    Ball();
    float x_pos, y_pos;
    float angle;
    float speed = 0.2f;
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
    Bonus() {};
    Bonus(int x, int y);
    sf::CircleShape ball;
    float x_pos, y_pos;
    bool remote;
    int type;
    float speed=0.3f;
    void move();
    ~Bonus() {};
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
    ~Game() {};
private:
    std::vector<std::vector<Block>> blocks;
    std::vector<Bonus> bonuses;
    Platform plat;
    Ball ball;
    void drawField();
    void createBlocks();
    void createPlatform();
    void IsTouchBallBlock();
    void Act(Block& b);
    void checkBonusCatch();
    void ActiveBonus(Bonus& bonus);
};


