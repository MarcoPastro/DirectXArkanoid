#pragma once
#include "GameObject.h"

class Block :
    public GameObject
{
public:
    /*
        @brief
        create the Block
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    Block(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x = 0, float y = 0);
    /*
        @brief
        create the Block
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param functionToDo the function that retorn the new position (x or y) in case of a edge collision
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    Block(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x = 0, float y = 0);
    /*
        @brief
        get the number of times that has been hit 
        @return the number of times that has been hit
    */
    int getNumBeingHit();
    /*
        @brief
        function that activate the logic of the block when been hit (in this case only increment the value)
    */
    virtual void Hit();

protected:
    int numBeingHit;
};

