#pragma once
#include "ArrayList.h" 
#include "DirectX.h"
#include <vector>
class Block;
class Ball;

class CollisionManager {
public:
    /*
        @brief
        create the Collision Manager empty
    */
    CollisionManager();
    /*
        @brief
        call the deconstractor of both arraylist
    */
    ~CollisionManager();
    /*
        @brief
        delete the old arraylist and take a pointer to the new ones
        @param other the other collision manager
    */
    CollisionManager& operator=(CollisionManager& other);
    /*
        @brief
        add the block to the specific arraylist
        @param blockToAdd block to add
    */
    void AddBlock(Block* blockToAdd);
    /*
        @brief
        add the blocks to the specific arraylist
        @param blocksToAdd blocks to add
        @param dim dimension of the array to add
    */
    void AddBlocks(Block** blocksToAdd, int dim);
    /*
        @brief
        add the ball to the specific arraylist
        @param ballToAdd ball to add
    */
    void AddBall(Ball* ballToAdd);
    /*
        @brief
        add the balls to the specific arraylist
        @param ballsToAdd balls to add
        @param dim dimension of the array to add
    */
    void AddBalls(Block** ballsToAdd, int dim);
    /*
        @brief
        checks the collision with 2 points that forms a rectangle
        @param x1Pos x of the up left point
        @param y1Pos y of the up left point
        @param x2Pos x of the down right point
        @param y2Pos y of the down right point
        @return 
    */
    int CheckCollision(GameObject* objectToCheck, float x1Pos, float y1Pos, float x2Pos, float y2Pos);
    /*
        @brief
        checks the collision with 2 points that forms a rectangle
        @param x1Pos x of the up left point
        @param y1Pos y of the up left point
        @param x2Pos x of the down right point
        @param y2Pos y of the down right point
    */
    bool CheckBallPosition(GameObject* objectToCheck, float x1Pos, float y1Pos, float x2Pos, float y2Pos);
    /*
        @brief
        get the arraylist of blocks
    */
    ArrayList* GetArrayListOfBlocks();
    /*
        @brief
        get the arraylist of balls
    */
    ArrayList* GetArrayOfBalls();
    /*
        @brief
        Draw the balls and the blocks
        @param dev the device
        @param devcon the device context
        @param pVBuffer the buffer
    */
    void Draw(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11Buffer* pVBuffer);

    /*void* GetAllVertex();
    void FinishedToAddElement();
    int GetDimVerticies();*/
private:
    class ArrayList* arrayListOfBlocks;
    class ArrayList* arrayOfBalls;
    float offsetYDetection = 0.005f ;
    float correction = 0.001f;

    /*VERTEX* allVertices;
    int dimVertices;*/
};
