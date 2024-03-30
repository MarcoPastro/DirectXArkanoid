#include "Block.h"

Block::Block(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x, float y)
    :GameObject(texture, collisionManager, velocity, x, y), numBeingHit(0){
}
Block::Block(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x, float y)
    :GameObject(texture, collisionManager,functionToDo, velocity, x, y), numBeingHit(0){
}

int Block::getNumBeingHit() {
    return numBeingHit;
}
void Block::Hit() {
    ++numBeingHit;
}

