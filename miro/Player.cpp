#include "Player.h"

Player::Player(int x, int y) : x(x), y(y) {}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

void Player::setX(int x) {
    this->x = x;
}
void Player::setY(int y) {
    this->y = y;
}

void Player::move(int dx, int dy) {
    x += dx;
    y += dy;
}