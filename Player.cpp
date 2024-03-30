#include "Player.h"

Player::Player(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x, float y)
    :Block(texture, collisionManager, velocity,x, y) {
}
Player::Player(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x, float y)
    :Block(texture, collisionManager, functionToDo, velocity,x, y) {
}


void Player::ChangeDirection(float x, float y) {
    _velY = _velocity * y; //_velY + _velocity * direction.GetY();
    _velX = _velocity * x; //_velX + _velocity * direction.GetX();
}

void Player::move(float timeStep) {
    float newX1 = _posX + _velX * timeStep;
    float newY1 = _posY + _velY * timeStep;
    float newX2 = newX1 + _textureWidth;
    float newY2 = newY1 + _textureHeight;
    if (collisionManager->CheckBallPosition(this, newX1, newY1, newX2, newY2) == true) return;
    moveY(timeStep);
    moveX(timeStep);
}