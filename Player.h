#pragma once
#include "Block.h"

class Player :
    public Block
{
public:
    /*
        @brief
        create the Player
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    Player(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x = 0, float y = 0);
    /*
        @brief
        create the Player
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    Player(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x = 0, float y = 0);
    /*
        @brief
        function that call moveX and moveY with a timeStep
        @param timeStep to change the movement
    */
    void move(float timeStep);
    /*
        @brief
        Takes key presses and adjusts the texture's velocity 
        @param sdlEvent the input
    */
    //void SetDirection(SDL_Event& sdlEvent);
    /*
        @brief
        Change the direction
        @param direction the direction
    */
    void ChangeDirection(float x, float y);
};

