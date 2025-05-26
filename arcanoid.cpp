#include "Header.h"
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arkanoid");

Block::Block(int i, int j) {
    x_pos =j * BLOCK_WIDTH;
    y_pos = i * BLOCK_HEIGHT;
    type = 1 + rand() % BLOCK_COUNT_TYPES;
    if (type == 4)
        life = LIFE_BONUS_4;
    else life = 1;
    block.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
    block.setOutlineThickness(2.f);
    block.setOutlineColor(sf::Color::Black);
    block.setPosition({ x_pos, y_pos });
    block.setFillColor(sf::Color::Blue);
    
}

Ball::Ball() {
    x_pos = (WINDOW_WIDTH- BALL_RADIUS) / 2;
    y_pos = (WINDOW_HEIGHT-BALL_RADIUS) / 2;
    angle = START_ANGLE * M_PI / 180.0f;
    ball.setRadius(BALL_RADIUS);
    ball.setFillColor(sf::Color::Red);
    ball.setOutlineThickness(2.f);
    ball.setOutlineColor(sf::Color::Black);
    ball.setPosition(x_pos, y_pos);
}

Platform::Platform() {
    platformWidth = 1.5 * BLOCK_WIDTH;
    x_pos = (WINDOW_WIDTH - platformWidth) / 2;
    y_pos = WINDOW_HEIGHT- BLOCK_HEIGHT;
    block.setSize({ platformWidth, BLOCK_HEIGHT });
    block.setOutlineThickness(2.f);
    block.setOutlineColor(sf::Color::Black);
    block.setFillColor(sf::Color::Red);
    block.setPosition({ x_pos, y_pos });
}

void Game::createBlocks() {
    blocks.clear();
    for (int i = 0; i < BLOCK_COUNT_HEIGHT; i++) {
        std::vector<Block*> row;
        for (int j = 0; j < BLOCK_COUNT_WIDTH; j++) {
            int type = 1 + rand() % BLOCK_COUNT_TYPES; // тип от 1 до 4

            Block* block = nullptr;

            switch (type) {
            case 1: // Неразрушимый
                block = new BlockUnbreakable(i, j);
                break;

            case 2: { // С бонусом
                int bonusType = 1 + rand() % BONUS_COUNT_TYPES; 
                block = new BlockWithBonus(i, j, bonusType);
                break;
            }

            case 3: { // Увеличение скорости
                float delta_v = 0.1f;
                block = new BlockSpeedUp(i, j, delta_v);
                break;
            }

            case 4: // С жизнями
                block = new BlockWithHealth(i, j);
                break;
            }

            row.push_back(block);
        }
        blocks.push_back(row);
    }
}

void BlockWithBonus::onHit(Game& game) {
    life--;
    game.Score_Win++;

    float bonus_x = x_pos + BLOCK_WIDTH / 2;
    float bonus_y = y_pos + BLOCK_HEIGHT / 2;

    Bonus* bonus = nullptr;
    switch (bonusType) {
    case 1: bonus = new BonusSpeed(bonus_x, bonus_y); break;
    case 2: bonus = new BonusResizePlatform(bonus_x, bonus_y); break;
    case 3: bonus = new BonusStick(bonus_x, bonus_y); break;
    case 4: bonus = new BonusSecondLife(bonus_x, bonus_y); break;
    case 5: bonus = new BonusRedirect(bonus_x, bonus_y); break;
    }
    if (bonus) game.bonuses.push_back(bonus);
}

void BlockSpeedUp::onHit(Game& game) {
    life--;
    game.Score_Win++;
    game.ball.speed += delta_v;
    if (game.ball.speed > MAX_SPEED)
        game.ball.speed = MAX_SPEED;
}

void BlockWithHealth::onHit(Game& game) {
    life--;
    game.Score_Win++;
}

void Platform::move(float x_cursor) {
    float left = x_pos;
    float right = x_pos + platformWidth;
    if (x_cursor < left && x_pos>0)
        x_pos -= speed;
    else if (x_cursor > right && right< WINDOW_WIDTH)
        x_pos += speed;
    block.setPosition(x_pos, y_pos);
}

void Ball:: verticalSides_Touch() {
    float right_side_x = WINDOW_WIDTH - 2 * BALL_RADIUS;
    if (x_pos < 0) {
        x_pos = 0;
        angle = M_PI - angle;
    }
    else if (x_pos > right_side_x) {
        x_pos = right_side_x;
        angle = M_PI - angle;
    }
}

void Ball::step() {
    y_pos = y_pos +  speed * sin(angle);
    x_pos = x_pos +  speed * cos(angle);
}

void Ball::Platform_Touch(bool& act,Platform& plat, int& Score_Fail) {

    float gran_bottom = WINDOW_HEIGHT - BLOCK_HEIGHT - 2 * BALL_RADIUS;
    float plat_left = plat.x_pos - BALL_RADIUS;
    float plat_right = plat.x_pos + plat.platformWidth - BALL_RADIUS;
    // Проигрыш: мяч улетел вниз
    if (y_pos >= WINDOW_HEIGHT - BLOCK_HEIGHT) {
        if (bottomPlatformActive) {
            bottomAct();
            return;
        }
        ball.setFillColor(sf::Color::Red);
        x_pos = (WINDOW_WIDTH - BALL_RADIUS) / 2;
        y_pos = (WINDOW_HEIGHT - BALL_RADIUS) / 2;
        angle = START_ANGLE * M_PI / 180.0f;
        Score_Fail++;
        act = true;
    }
    // Отражение от платформы
    else if (y_pos >= gran_bottom && x_pos >= plat_left && x_pos <= plat_right) {
        angle = -angle - VOZM;
        step();
        if (isStuckToPlatform) {
            stuckAct(plat);
        }
        act = true;
    }

}

void Ball::bottomAct() {
    angle = -angle - VOZM;
    y_pos = WINDOW_HEIGHT - BLOCK_HEIGHT - 2 * BALL_RADIUS;
    bottomPlatformActive = false;
};

void Ball::stuckAct(Platform& plat) {
    x_pos = plat.x_pos + plat.platformWidth / 2 - BALL_RADIUS;
    y_pos = plat.y_pos - 2 * BALL_RADIUS;
    ball.setPosition(x_pos, y_pos);
    if (stickClock.getElapsedTime() >= stickDuration) {
        isStuckToPlatform = false;
    }
}

void Ball::RedirectAct() {
    if (randomRedirectClock.getElapsedTime() >= randomRedirectTime) {
        float degrees = 30 + rand() % 120;
        angle = degrees * M_PI / 180.0f;
        randomRedirectActive = false;
    }
}

void Ball::move(Platform& plat, int& Score_Fail) {
    //верхняя стенка
    if (y_pos < 0) {
        y_pos = 0;
        angle = -angle;
    }

    bool act_platform = false;
    Platform_Touch(act_platform,plat, Score_Fail);
    if(!act_platform) {
        step();// Свободный полёт
    }

    verticalSides_Touch();

    if (randomRedirectActive)
        RedirectAct();

    ball.setPosition(x_pos, y_pos);
}
   
Game::Game() {
    Score_Fail = 0;
    Score_Win = 0;
    createBlocks();
}

void Game::IsTouchBallBlock() {
    float ballLeft = ball.x_pos;
    float ballRight = ball.x_pos + 2 * BALL_RADIUS;
    float ballTop = ball.y_pos;
    float ballBottom = ball.y_pos + 2 * BALL_RADIUS;

    for (int i = 0; i < BLOCK_COUNT_HEIGHT; ++i) {
        for (int j = 0; j < BLOCK_COUNT_WIDTH; ++j) {
            Block* b = blocks[i][j];

            if (b->life <= 0 && b->type != 1)
                continue;

            float blockLeft = b->x_pos;
            float blockRight = b->x_pos + b->block.getSize().x;
            float blockTop = b->y_pos;
            float blockBottom = b->y_pos + BLOCK_HEIGHT;

            // Проверка пересечения
            if (ballRight > blockLeft && ballLeft < blockRight &&
                ballBottom > blockTop && ballTop < blockBottom) {

                // === 1. Обработка отражения ===
                float overlapLeft = ballRight - blockLeft;
                float overlapRight = blockRight - ballLeft;
                float overlapTop = ballBottom - blockTop;
                float overlapBottom = blockBottom - ballTop;

                float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

                if (minOverlap == overlapLeft) {
                    ball.x_pos -= overlapLeft;
                    ball.angle = M_PI - ball.angle;
                }
                else if (minOverlap == overlapRight) {
                    ball.x_pos += overlapRight;
                    ball.angle = M_PI - ball.angle;
                }
                else if (minOverlap == overlapTop) {
                    ball.y_pos -= overlapTop;
                    ball.angle *= -1.0f;
                }
                else {
                    ball.y_pos += overlapBottom;
                    ball.angle *= -1.0f;
                }

                ball.ball.setPosition(ball.x_pos, ball.y_pos);

            
                b->onHit(*this);

                return; 
            }
        }
    }
}


Bonus::Bonus(float x, float y) : x_pos(x), y_pos(y) {
    ball.setRadius(BONUS_RADIUS);
    ball.setFillColor(sf::Color::Yellow);
    ball.setOutlineThickness(1.f);
    ball.setOutlineColor(sf::Color::Black);
    ball.setPosition(x_pos, y_pos);
}

void Bonus::move() {
    if (y_pos < WINDOW_HEIGHT)
        y_pos += speed;
    else
        remote = true;
    ball.setPosition(x_pos, y_pos);
}

void BonusSpeed::apply(Game& game) {
    if (game.ball.speed <= MAX_SPEED)
        game.ball.speed = (1 + rand() % 10) * 0.1f;
}

void BonusResizePlatform::apply(Game& game){
    game.plat.platformWidth *= (0.5f + (rand() % 11) * 0.1f);
    game.plat.block.setSize({ game.plat.platformWidth, BLOCK_HEIGHT });
}

void BonusStick::apply(Game& game) {
    game.ball.isStuckToPlatform = true;
    game.ball.stickClock.restart();
}

void BonusSecondLife::apply(Game& game) {
    bottomPlatformActive = true;
    bottomClock.restart();
}

void BonusRedirect::apply(Game& game) {
    game.ball.randomRedirectActive = true;
    int ms = 1000 + rand() % 3000; // 1-4 секунды
    game.ball.randomRedirectTime = sf::milliseconds(ms);
    game.ball.randomRedirectClock.restart();
}

void Game::run() {
    while (window.isOpen()) {
        window.clear();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (bottomClock.getElapsedTime() >= bottomDuration) {
            bottomPlatformActive = false;
        }
        plat.move(mousePos.x);
        checkBonusCatch();
        ball.move(plat, Score_Fail);
        for (Bonus* bonus : bonuses) {
            if (!bonus->remote) {
                bonus->move();
            }
        }
        IsTouchBallBlock();
        UpdateBlocks();
        drawField();
        window.display();
    }
}

void Game::ActiveBonus(Bonus* bonus) {

    float ballLeft = bonus->x_pos;
    float ballRight = bonus->x_pos + 0.5 * BALL_RADIUS;

    if (ballRight >= plat.x_pos && ballLeft <= plat.x_pos + plat.platformWidth && bonus->y_pos >= plat.y_pos) {
        bonus->remote = true;
        bonus->apply(*this);
    }
}

void Game::checkBonusCatch() {
    for (Bonus* bonus : bonuses) {
        if (!bonus->remote) {
            ActiveBonus(bonus);
        }
    }
}

void Game::UpdateBlocks() {
    for (std::vector<Block*>& row : blocks) {
        for (Block* b : row) {
            if (b->life <= 0 && b->type != 1) {
                b->block.setFillColor(sf::Color::Transparent);
            }
        }
    }
}

void Game::drawField() {
    for (int i = 0; i < BLOCK_COUNT_HEIGHT; ++i) {
        for (int j = 0; j < BLOCK_COUNT_WIDTH; ++j) {
            if (blocks[i][j]->life!=0)
                window.draw(blocks[i][j]->block);
        }
    }
    window.draw(plat.block);
    window.draw(ball.ball);

    for (Bonus* bonus : bonuses) {
        if(!bonus->remote)
        window.draw(bonus->ball);
    }

    if (bottomPlatformActive) {
        sf::RectangleShape bottom;
        bottom.setSize({ WINDOW_WIDTH, 2 });
        bottom.setPosition(0, WINDOW_HEIGHT - BLOCK_HEIGHT);
        bottom.setFillColor(sf::Color::Green);
        window.draw(bottom);
    }

    std::cout << "Score_Fail: " << Score_Fail  << "Score_Win: " << Score_Win << "\r" << std::flush;

}

Game::~Game() {
    for (std::vector<Block*>& row : blocks) {
        for (Block* b : row) {
            delete b;
        }
    }
    for (Bonus* b : bonuses)
        delete b;
}

int main() {
    srand(time(0));
    Game PlayGame;
    PlayGame.run();
}
