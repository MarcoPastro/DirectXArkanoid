#pragma once
// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <iostream>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")



// define the screen resolution
#define SCREEN_WIDTH  600	
#define SCREEN_HEIGHT 600

#define PIGRECO 3.14159f

#define BLOCKWIDTH 0.2f
#define BLOCKHEIGHT 0.1f

#define BALLWIDTH 0.08f //it breaks collisions if too low 
#define BALLHEIGHT 0.08f

#define COLORHIT1 D3DXCOLOR(0.5f, 0.2f, 0.5f, 1.0f)
#define COLORHIT2 D3DXCOLOR(0.7f, 0.2f, 0.2f, 1.0f)
#define COLORHIT3 D3DXCOLOR(1.f, 0.f, 0.f, 1.0f)

#define COLORPLAYER D3DXCOLOR(1.f, 1.f, 1.f, 1.0f)
#define COLORBALL D3DXCOLOR(0.f, 0.8f, 1.f, 1.0f)
struct VERTEX { FLOAT X, Y, Z; D3DXCOLOR Color; };
