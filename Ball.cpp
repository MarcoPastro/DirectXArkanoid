#include "Ball.h"

Ball::Ball(D3DXCOLOR texture, CollisionManager* collisionManager, float velocity, float x, float y)
    :GameObject(texture, collisionManager, velocity, x, y) {
    _textureHeight = BALLHEIGHT;
    _textureWidth = BALLWIDTH;
}
Ball::Ball(D3DXCOLOR texture, CollisionManager* collisionManager, float (*functionToDo)(float), float velocity, float x, float y)
    :GameObject(texture, collisionManager, functionToDo, velocity, x, y) {
    _textureHeight = BALLHEIGHT;
    _textureWidth = BALLWIDTH;
}

void Ball::Bounce(bool bounceX, bool bounceY) {
    if (bounceX) {
        _velX = -_velX;
    }
    if (bounceY) {
        _velY = -_velY;
    }
}

void Ball::move(float timeStep) {
    float newX1 = _posX + _velX * timeStep;
    float newX2 = newX1 + _textureWidth;
    float newY1 = _posY + _velY * timeStep;
    float newY2 = newY1 + _textureHeight;
    float collRes = collisionManager->CheckCollision(this, newX1, newY1, newX2, newY2);
    if (collRes < 0) {
        Bounce(false, true);
    }
    else if (collRes > 0) {
        Bounce(true,false);
    }
     moveY(timeStep);
     moveX(timeStep);
    
    
}
void Ball::moveX(float timeStep) {
    // Move the texture left or right
    _posX += _velX * timeStep;

    // If the texture went too far to the left or right
    if (_posX < -1.f) {
        _posX = functionToDoOnEdge(-1.f);
        Bounce(1, 0);
    }
    else if (_posX > 1.f - _textureWidth) {
        _posX = functionToDoOnEdge(1.f - _textureWidth);
        Bounce(1, 0);
    }
}
void Ball::moveY(float timeStep) {
    // Move the texture up or down
    _posY += _velY * timeStep;

    // If the texture went too far up or down
    if (_posY < -1.f) {
        setActive(false);
    }
    else if (_posY > 1.f) {
        _posY = functionToDoOnEdge(1.f);
        Bounce(0, 1);
    }
}
/*void Ball::render(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11Buffer* pVBuffer) {
    // Show the texture
    if (active) {
        const int segments = 36;
        float squareCenterX = (_posX + _textureWidth) / 2.0f;
        float squareCenterY = (_posY - _textureHeight) / 2.0f;

        // Calculate the radius of the circle to fit within the square
        float circleRadius = _textureWidth / 2.0f;

        // Create vertices for the circle
        VERTEX vertices[segments];

        // Calculate angle increment
        float angleIncrement = PIGRECO / segments;

        // Generate vertices for the circle
        for (int i = 0; i < segments; ++i) {
            float angle = angleIncrement * i;
            float x = squareCenterX + circleRadius * cos(angle);
            float y = squareCenterY + circleRadius * sin(angle);
            vertices[i] = { x,y,0.0f,texture };
        }

        UINT stride = sizeof(VERTEX);
        UINT offset = 0;

        D3D11_MAPPED_SUBRESOURCE ms;
        devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);      // map the buffer
        memcpy(ms.pData, vertices, sizeof(vertices));                                                         // copy the data
        devcon->Unmap(pVBuffer, NULL);

        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

        // select which primtive type we are using
        //devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        // draw the vertex buffer to the back buffer
        devcon->Draw(segments, 0);
    }
}*/