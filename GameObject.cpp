#include "GameObject.h"

GameObject::GameObject(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x, float y)
    : texture(texture)
    , collisionManager(collisionManager)
    , _posX(x)
    , _posY(y)
    , _velocity(velocity)
    , active(true) {
    _velX = _velocity;
    _velY = _velocity;
    functionToDoOnEdge = [](float edge) {
        return edge;
    };
    _textureWidth = BLOCKWIDTH;
    _textureHeight = BLOCKHEIGHT;

    vertices = new VERTEX[4];//four verticies
}

GameObject::GameObject(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x, float y)
    : texture(texture)
    , collisionManager(collisionManager)
    , _posX(x)
    , _posY(y)
    , functionToDoOnEdge(functionToDoOnEdge)
    , _velocity(velocity)
    , active(true) {
    _velX = _velocity;
    _velY = _velocity;
    _textureWidth = BLOCKWIDTH;
    _textureHeight = BLOCKHEIGHT;
    vertices = new VERTEX[4];//four verticies
}


void GameObject::moveX(float timeStep) {
    // Move the texture left or right
    _posX += _velX * timeStep;

    // If the texture went too far to the left or right
    if (_posX < -1.f) {
        _posX = functionToDoOnEdge(-1.f);
    }
    else if (_posX > 1.f - _textureWidth) {
        _posX = functionToDoOnEdge(1.f - _textureWidth);
    }
}

void GameObject::moveY(float timeStep) {
    
    // Move the texture up or down
    _posY += _velY * timeStep;
    
    // If the texture went too far up or down
    if (_posY < -1.f) {
        _posY = functionToDoOnEdge(-1.f);
    }
    else if (_posY > 1.f - _textureHeight) {
        _posY = functionToDoOnEdge(1.f - _textureHeight);
    }
}


void GameObject::move(float timeStep) {
    float newX1 = _posX + _velX * timeStep;
    float newY1 = _posY + _velY * timeStep;
    float newX2 = newX1 + _textureWidth;
    float newY2 = newY1 + _textureHeight;
    //if (collisionManager->CheckCollision(this, newX1, newY1, newX2, newY2)>0) return;
    moveY(timeStep);
    moveX(timeStep);
}
void GameObject::render(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11Buffer* pVBuffer) {
    // Show the texture
    if (active) {

        vertices[0] = {_posX, _posY - _textureHeight, 0.0f, texture};
        vertices[1] = { _posX, _posY, 0.0f, texture };
        vertices[2] = { _posX + _textureWidth, _posY - _textureHeight, 0.0f, texture };
        vertices[3] = { _posX + _textureWidth, _posY, 0.0f, texture };
        
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        D3D11_MAPPED_SUBRESOURCE ms;
        devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);      // map the buffer
        memcpy(ms.pData, vertices, sizeof(VERTEX)*4);                                                         // copy the data
        devcon->Unmap(pVBuffer, NULL);

        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

        // select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        // draw the vertex buffer to the back buffer
        devcon->Draw(4, 0);


    }
}
// Sets the position of this texture (0,0 is the top left)
void GameObject::setPosX(float posX) {
    _posX = posX;
}
void GameObject::setPosY(float posY) {
    _posY = posY;
}

// Gets the position of this texture (0,0 is the top left)
const float GameObject::getPosX() {
    return _posX;
}
const float GameObject::getPosY() {
    return _posY;
}

// Sets the general velocity of this texture with a multiplier
void GameObject::setVelocity(float velocity) {
    _velocity = velocity;
    _velX = velocity;
    _velY = velocity;
}

float GameObject::getVelocity() {
    return _velocity;
}

// Sets the current velocity of this texture
void GameObject::setVelX(float velX) {
    _velX = velX;
}
void GameObject::setVelY(float velY) {
    _velY = velY;
}

// Gets the current velocity of this texture
const float GameObject::getVelX() {
    return _velX;
}
const float GameObject::getVelY() {
    return _velY;
}

// Gets the current velocity of this texture
const float GameObject::getTextureWidth() {
    return _textureWidth;
}
const float GameObject::getTextureHeight() {
    return _textureHeight;
}

void GameObject::ChangeTexture(D3DXCOLOR newTexture)
{
    texture = newTexture;
}

void GameObject::setActive(bool value) {
    active = value;
}
bool GameObject::getActive() {
    return active;
}
D3DXCOLOR GameObject::GetTexture() {
    return texture;
}
/*
VERTEX* GameObject::getVertices()
{
    return vertices;
}

int GameObject::getDimVertecies()
{
    return 4;//return the num of verticies
}*/

GameObject::~GameObject() {
    delete[] vertices;
}