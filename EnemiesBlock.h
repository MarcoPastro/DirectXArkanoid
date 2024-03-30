#pragma once
#include "Block.h"

class EnemiesBlock :
    public Block
{
public:
    /*
        @brief
        create the enemy block
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    EnemiesBlock(D3DXCOLOR texture, CollisionManager* collisionManager, int lifeOfTheBlock = 1, float x = 0, float y = 0);
    /*
        @brief
        create the Block
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param functionToDo the function that retorn the new position (x or y) in case of a edge collision
        @param lifeOfTheBlock the initial life of the block
        @param x the x starting position
        @param y the y starting position
    */
    EnemiesBlock(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), int lifeOfTheBlock = 1, float x = 0, float y = 0);
    /*
        @brief
        function that activate the logic of the block when been hit (in this case only increment the value)
    */
    void Hit();
    /*
        @brief
        reset the life with the start value
        @param newlife the new life of the block
    */
    void ResetLife(int newlife = -1);
private:
    int life;
    int startLife;
};
