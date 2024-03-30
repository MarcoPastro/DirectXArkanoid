#pragma once
#include "DirectX.h"
#include "CollisionManager.h"
#include <string>


class GameObject {
public:
    /*
        @brief
        create a gameobject
        @param texture the name of the texture
        @param collisionManager where the collision can be checked
        @param velocity the initial velocity of the ball
        @param x the x starting position
        @param y the y starting position
    */
    GameObject(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x = 0, float y = 0);

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
    GameObject(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x = 0, float y = 0);

    ~GameObject();

    // Moves the texture
    /*
        @brief
        Moves the texture based on a timeStep
        @param timeStep the time step
    */
    virtual void move(float timeStep);

    // Moves the texture on X axis
    /*
        @brief
        Moves the texture on a x axis based on a timeStep
        @param timeStep the time step
    */
    virtual void moveX(float timeStep);

    // Moves the texture on Y axis
    /*
        @brief
        Moves the texture on a y axis based on a timeStep
        @param timeStep the time step
    */
    virtual void moveY(float timeStep);

    /*
        @brief
        Shows the texture on the screen
    */
    virtual void render(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11Buffer* pVBuffer);

    // Sets the position of this texture (0,0 is the top left)
    /*
        @brief
        Sets the position on the x axis of this texture (0,0 is the top left)
        @param timeStep the time step
    */
    void setPosX(float posX);
    /*
        @brief
        Sets the position on the y axis of this texture (0,0 is the top left)
        @param timeStep the time step
    */
    void setPosY(float posY);

    // Gets the position of this texture (0,0 is the top left)
    /*
        @brief
        Gets the position on the x axis of this texture (0,0 is the top left)
        @return the x position
    */
    const float getPosX();
    /*
        @brief
        Gets the position on the y axis of this texture (0,0 is the top left)
        @return the x position
    */
    const float getPosY();

    // Sets the general velocity of the texture
    /*
        @brief
        Sets the general velocity of the texture
        @param velocity the new value
    */
    void setVelocity(float velocity);

    /*
        @brief
        Gets the general velocity of the texture
        @return the velocity
    */
    float getVelocity();

    // Sets the current velocity of the x or y of this texture
    /*
        @brief
        Sets the current velocity of the x of this texture
        @param the x position
    */
    void setVelX(float velX);
    /*
        @brief
        Sets the current velocity of the y of this texture
        @param the x position
    */
    void setVelY(float velY);

    // Gets the current velocity of this texture
    /*
        @brief
        Gets the current velocity of the x of this texture
        @return the velocity
    */
    const float getVelX();
    /*
        @brief
        Gets the current velocity of the y of this texture
        @return the velocity
    */
    const float getVelY();

    /*
        @brief
        Gets the width of the texture
        @return the width of the texture
    */
    const float getTextureWidth();
    /*
        @brief
        Gets the height of the texture
        @return the height of the texture
    */
    const float getTextureHeight();

    // Change the texture
    /*
        @brief
        Change the texture
        @param nameTexture the name of the texture
    */
    void ChangeTexture(D3DXCOLOR newTexture);

    /*
        @brief
        Set the object to active (if not active the render and the collision doesn't work)
        @param value the value
    */
    void setActive(bool value);
    /*
        @brief
        Gets true or false if the object is active
        @return the value of active
    */
    bool getActive();
    /*
        @brief
        Gets the texture
        @return the texture
    */
    D3DXCOLOR GetTexture();

    /*VERTEX* getVertices();

    int getDimVertecies();*/
protected:
    VERTEX* vertices;

    float _posX, _posY;
    float _velX, _velY;

    // The dimensions of the texture
    float _textureWidth = 0.1f;
    float _textureHeight = 0.1f;

    // Maximum axis velocity of the texture
    float _velocity = 0.1f;

    float (*functionToDoOnEdge)(float);

    class CollisionManager* collisionManager;

    bool active;
    
    D3DXCOLOR texture;
};