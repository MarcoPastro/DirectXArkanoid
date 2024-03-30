#include "EnemiesBlock.h"

EnemiesBlock::EnemiesBlock(D3DXCOLOR texture, CollisionManager* collisionManager, int life, float x, float y)
    :Block(texture, collisionManager, 0.f, x, y), startLife(life), life(life){
}
EnemiesBlock::EnemiesBlock(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), int life, float x, float y)
    :Block(texture, collisionManager, functionToDo, 0.f, x, y), startLife(life), life(life) {
}

void EnemiesBlock::Hit() {
    ++numBeingHit;
    if (numBeingHit >= life) {
        setActive(false);
    }
    else {
        switch (life - numBeingHit) {
        case 1:
            ChangeTexture(COLORHIT1);
            break;
        case 2:
            ChangeTexture(COLORHIT2);
            break;
        case 3:
            ChangeTexture(COLORHIT3);
            break;
        }
    }
}

void EnemiesBlock::ResetLife(int newlife)
{
    numBeingHit = 0;
    if (newlife > 0 && newlife <= 3) {
        startLife = newlife;
    }
    life = startLife;
    switch (life) {
    case 1:
            ChangeTexture(COLORHIT1);
            break;
        case 2:
            ChangeTexture(COLORHIT2);
            break;
        case 3:
            ChangeTexture(COLORHIT3);
            break;
    }
}