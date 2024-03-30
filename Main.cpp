// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <cstdio> //Some utilities for the log
#include <memory> // Needed for the smart pointers
#include <iostream>
#include <random>
#include <chrono>
#include "Player.h"
#include "CollisionManager.h"
#include "Ball.h"
#include "EnemiesBlock.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")




// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface
ID3D11Device* dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
ID3D11InputLayout* pLayout;            // the pointer to the input layout
ID3D11VertexShader* pVS;               // the pointer to the vertex shader
ID3D11PixelShader* pPS;                // the pointer to the pixel shader
ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer

float dX= 0;
// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(CollisionManager* collisionManager);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitPipeline(void);    // loads and prepares the shaders

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/*
        @brief
        get a random number in a range
        @param min the min number of the range
        @param max the max number of the range
        @return the random value in the range
*/
int getRandomNumber(int min, int max) {
    return min + (rand() % (max - min));
}
/*
        @brief
        get a random number in a range
        @param min the min number of the range
        @param max the max number of the range
        @return the random value in the range
*/
float  getRandomNumber(float  min, float  max) {
    // Seed the random number generator
    

    // Generate a random integer between 0 and RAND_MAX
    int randomInt = rand();

    // Scale the random integer to fit within the range [min, max]
    float randomValue = min + static_cast<float>(randomInt) / RAND_MAX * (max - min);

    return randomValue;
}
/*
        @brief
        get a random number in a range
        @param numBlocks number of blocks
        @param spaceForThePlayer the space to subtract for the player
        @param collisionManager the collisionManager manager for each block
        @return the array of blocks with a random location in a window
*/
EnemiesBlock** CreateRandomBlocks(int numBlocks, float spaceForThePlayer, CollisionManager* collisionManager) {
    EnemiesBlock** enemiesArray = new EnemiesBlock * [numBlocks];
    float realWindowHeight = 2.f - spaceForThePlayer;

    int numCols = 1;
    int numRows = numBlocks;

    for (int i = 2; i <= sqrt(numBlocks); ++i) {
        if (numBlocks % i == 0) {
            numCols = i;
            numRows = numBlocks / i;
            break;
        }
    }

    for (int i = 0; i < numBlocks; ++i) {
        int life = getRandomNumber(1, 4);//possible Blocks
        D3DXCOLOR textureName;
        switch (life) {
        case 1:
            textureName = COLORHIT1;
            break;
        case 2:
            textureName = COLORHIT2;
            break;
        case 3:
            textureName = COLORHIT3;
            break;
        }
        enemiesArray[i] = new EnemiesBlock(textureName, collisionManager, life);
    }

    /*float rectangleSizeX = 2.f / numRows;
    float rectangleSizeY = realWindowHeight / numCols;*/

    int i = 0;
    for (float yScreen = 1.f - BLOCKHEIGHT; yScreen > spaceForThePlayer; yScreen = yScreen - BLOCKHEIGHT*2) {
        for (float xScreen = -1.f + BLOCKWIDTH + BLOCKWIDTH/2.f; xScreen < 1.f - BLOCKWIDTH; xScreen = xScreen + BLOCKWIDTH*2) {
            if (i < numBlocks) {
                enemiesArray[i]->setPosX(xScreen);
                enemiesArray[i]->setPosY(yScreen);
                ++i;
            }
        }
    }
    /*for (int col = 0; col < numCols; ++col) {
        for (int row = 0; row < numRows; ++row) {
            if (i < numBlocks) {
                float x = row * rectangleSizeX;
                float y = col * rectangleSizeY;
                enemiesArray[i]->setPosX(getRandomNumber(x, x + rectangleSizeX - 0.1f)-1.f);
                enemiesArray[i]->setPosY(getRandomNumber(y, y + rectangleSizeY));
                ++i;
            }
        }
    }*/
    return enemiesArray;
}

float tick(std::chrono::high_resolution_clock::time_point &lastFrameTime) {
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    float deltaTimeSeconds = deltaTime.count();

    return deltaTimeSeconds;
}

bool isQuittingGame = false;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{

    // Game loop flag
    bool restart = false;
    bool gameOver = false;

    int numberOfBlocksPerLevels = 12;

    const float playerStartingPositionX = 0.f;
    const float playerStartingPositionY = -0.8f;

    const float velocityPlayer = 1.f;
    const float velocityBall = 1.f;

    srand(time(NULL));

    CollisionManager* collisionManager = new CollisionManager();
    EnemiesBlock** enemiesBlocks = CreateRandomBlocks(numberOfBlocksPerLevels, -0.05f, collisionManager);

    D3DXCOLOR playerColor = COLORPLAYER;
    D3DXCOLOR ballColor = COLORBALL;

    Player* player = new Player(playerColor, collisionManager, velocityPlayer, playerStartingPositionX, playerStartingPositionY);
    player->setVelX(0.0f);
    player->setVelY(0.0f);
    Ball* ball = new Ball(ballColor, collisionManager, velocityBall, playerStartingPositionX, 0.f);
    ball->setVelY((ball->getVelocity()));

    collisionManager->AddBlock(player);
    collisionManager->AddBall(ball);
    for (int i = 0; i < numberOfBlocksPerLevels; ++i) {
        collisionManager->AddBlock(enemiesBlocks[i]);
    }
    //collisionManager->FinishedToAddElement();
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
        L"WindowClass",
        L"Our First Direct3D Program",
        WS_OVERLAPPEDWINDOW,
        300,
        300,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

    // enter the main loop:

    MSG msg;
    float fixedTimeStep = 0.01f;
    float basetime = 0.007f;
    std::chrono::high_resolution_clock::time_point lastFrameTime = std::chrono::high_resolution_clock::now();
    while (!isQuittingGame)
    {
            fixedTimeStep = tick(lastFrameTime);
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT)
                    break;
            }
            player->ChangeDirection(dX, 0.f);
            player->move(fixedTimeStep);
            ball->move(fixedTimeStep);
            RenderFrame(collisionManager);
            {
                bool finish = true;
                bool newLevel = false;
                for (int i = 0; i < numberOfBlocksPerLevels && finish; ++i) {
                    if (enemiesBlocks[i]->getActive()) {
                        finish = false;
                    }
                }
                
                if (finish || ball->getActive() == false) {

                    for (int i = 0; i < numberOfBlocksPerLevels; ++i) {
                        enemiesBlocks[i]->setActive(true);
                        if (finish) {
                            enemiesBlocks[i]->ResetLife(getRandomNumber(1, 4)); // create a new map
                            
                        }
                        else {
                            enemiesBlocks[i]->ResetLife(); //restart with the previous map
                        }
                        
                    }
                    ball->setActive(true);
                    ball->setPosX(playerStartingPositionX);
                    ball->setPosY(0.0f);
                    if (finish) {
                        float prevVel = ball->getVelocity();
                        float newVel = prevVel + prevVel * 10 / 100; //increment the ball speed by 10%
                        ball->setVelocity(newVel);

                        prevVel = player->getVelocity();
                        newVel = prevVel + prevVel * 5 / 100; //increment the ball speed by 5%
                        player->setVelocity(newVel);
                    }
                    
                    ball->setVelY((ball->getVelocity()));
                    player->setPosX(playerStartingPositionX);
                }
            }
    }

    delete player;
    player = nullptr;
    delete ball;
    ball = nullptr;
    for (int i = 0; i < numberOfBlocksPerLevels; ++i) {
        delete enemiesBlocks[i];
        enemiesBlocks[i] = nullptr;
    }
    delete[] enemiesBlocks;
    enemiesBlocks = nullptr;

    delete collisionManager;
    //clear the screen
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 4;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    devcon->Unmap(pVBuffer, NULL);
    // clean up DirectX and COM
    CleanD3D();
    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    } break;
    case WM_KEYDOWN:
    {
        if (wParam == VK_LEFT || wParam == VK_RIGHT)
        {
            if (wParam == VK_LEFT) {
                dX = -1;
            }
            else {
                dX = 1;
            }
        }
        if (wParam == VK_ESCAPE) {
            isQuittingGame = true;
        }
    } break;
    case WM_KEYUP:
    {
        dX = 0;
    }break;

    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);


    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);



    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 4;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);

    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = SCREEN_WIDTH;
    viewport.Height = SCREEN_HEIGHT;

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
}
// this is the function used to render a single frame
void RenderFrame(CollisionManager* collisionManager)
{

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    collisionManager->Draw(dev, devcon, pVBuffer);

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}


// this function loads and prepares the shaders
void InitPipeline()
{
    // load and compile the two shaders
    ID3D10Blob* VS, * PS;
    D3DX11CompileFromFile(L"shader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}
