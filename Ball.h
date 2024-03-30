#pragma once
#include "GameObject.h"

class Ball :
    public GameObject
{
public:
    /*
        @brief
        create the ball
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    Ball(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x = 0, float y = 0);
    /*
        @brief
        create the ball
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param functionToDo the function that retorn the new position (x or y) in case of a edge collision
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    Ball(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x = 0, float y = 0);

    /*
        @brief
        function that change the velocity in x or in y based on the input
        @param bounceX if the velocity in x has to change
        @param bounceY if the velocity in y has to change
    */
    void Bounce(bool bounceX, bool bounceY);

    /*
        @brief
        function that call moveX and moveY with a timeStep
        @param timeStep to change the movement
    */
    void move(float timeStep);
    /*
        @brief
        the movement based on the timeStep in the x axis
        @param timeStep to change the movement
    */
    void moveX(float timeStep);
    /*
        @brief
        the movement based on the timeStep in the y axis
        @param timeStep to change the movement
    */
    void moveY(float timeStep);
};
