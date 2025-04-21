#include "player.h"

Player::Player():points(0){
};

void Player::updatePlayerPoint() {
    this->points += 1;
}

int Player::getPlayerPoint() const{
    return points;
}

