#include "CollisionManager.h"
#include "Ball.h"
#include "Block.h"
/*
@brief
check if two floats are equals based on a precision.
@param f1 the first float.
@param f2 the second float.
@param precisions the precision of the operation set by default 0.001f.
@return True if the f1 and the f2 are equals with some precision.
*/
bool isNearlyEquals(float f1, float f2, float precisions = 0.0001f)
{
	float fr = f2 - f1;
	if (fr < 0) {
		fr = -fr;
	}
	return (fr <= precisions);
}

//function that check if point xp and yp if is inside the sqaure x1,y1,x2,y2 true else false(<)(default x1=0,y1=0,x2=0,y3=0)(use the function isNearlyEquals
/*
@brief
check if the point is inside or on the edge of the rectangle and the default is the rectangle x1=0,y1=0,x2=1,y2=1.
@param xp the x of the point.
@param yp the y of the point.
@param x1 left side of the rectangle and the default is 0.
@param x2 right side of the rectangle.
@param y1 bottom side of the rectangle.
@param y2 top side of the rectangle.
@return True if is inside on the edge of the rectangle considerating the perimeter of the rectangle.
*/
int isPointInsideASquare(float xp, float yp, float x1, float x2, float y1, float y2, float offsetYDetection)
{
	if (!(   (((yp > y1) && (yp < y2)) 
			 || isNearlyEquals(yp, y1) 
		     || isNearlyEquals(yp, y2)) && (((xp > x1) && (xp < x2)) 
											|| isNearlyEquals(xp, x1) 
											|| isNearlyEquals(xp, x2)) )) {
		return 0;
	}
	if (yp - y1 < offsetYDetection || y2 - yp < offsetYDetection) {
		return -1;
	}
	return 1;
}


CollisionManager::CollisionManager() {
	arrayListOfBlocks = new ArrayList();
	arrayOfBalls = new ArrayList();
}

CollisionManager::~CollisionManager() {
	delete arrayListOfBlocks;
	delete arrayOfBalls;
	//delete[] allVertices;
}

CollisionManager& CollisionManager::operator=(CollisionManager& other)
{
	if (this != &other) {
		delete arrayListOfBlocks;
		delete arrayOfBalls;
		arrayListOfBlocks = other.GetArrayListOfBlocks();
		arrayOfBalls = other.GetArrayOfBalls();
	}
	return *this;
}

ArrayList* CollisionManager::GetArrayListOfBlocks() {
	return arrayListOfBlocks;
}
ArrayList* CollisionManager::GetArrayOfBalls() {
	return arrayOfBalls;
}

void CollisionManager::Draw(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11Buffer* pVBuffer)
{
	/*D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 4;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	*/
	int sizeArray = arrayOfBalls->GetSize();
	for (int i = 0; i < sizeArray; ++i) {
		Ball* b = (Ball*)arrayOfBalls->Get(i);
		b->render(dev, devcon, pVBuffer);
	}

	sizeArray = arrayListOfBlocks->GetSize();
	for (int i = 0; i < sizeArray; ++i) {
		Block* b = (Block*)arrayListOfBlocks->Get(i);
		b->render(dev, devcon, pVBuffer);
	}

	
	/*
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);      // map the buffer
	memcpy(ms.pData, allVertices, sizeof(allVertices));                                                         // copy the data
	devcon->Unmap(pVBuffer, NULL);

	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// select which primtive type we are using
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	// draw the vertex buffer to the back buffer
	devcon->Draw(dimVertices, 0);
	*/
}

void CollisionManager::AddBlock(Block* blockToAdd) {
	arrayListOfBlocks->Add(blockToAdd);
}
void CollisionManager::AddBlocks(Block** blocksToAdd, int dim) {
	for (int i = 0; i < dim; ++i) {
		arrayListOfBlocks->Add(blocksToAdd[i]);
	}
}
void CollisionManager::AddBall(Ball* ballToAdd) {
	arrayOfBalls->Add(ballToAdd);
}
void CollisionManager::AddBalls(Block** ballsToAdd, int dim) {
	for (int i = 0; i < dim; ++i) {
		arrayOfBalls->Add(ballsToAdd[i]);
	}
}
bool getResetPos(float tocheck1, float tocheck2, float pos1, float pos2) {
	if (tocheck1 + tocheck2 > pos1 + pos2) {
		return true;
	}
	return false;
}
bool isAxesInsideASquare(float xp, float x1, float x2)
{
	if (x2 > xp && x1 < xp || isNearlyEquals(xp, x1) || isNearlyEquals(xp, x2)) {
		return true;
	}
	return false;
}
int CollisionManager::CheckCollision(GameObject* objectToCheck, float x1Pos, float y1Pos, float x2Pos, float y2Pos) {
	int sizeArray = arrayListOfBlocks->GetSize();
	float v1x, v1y, v2x, v2y;
	float xChoosed = x1Pos;
	float yChoosed = y1Pos;
	int result = 0;
	bool changed = false;
	bool passedFirst = false;
	bool needtoCheck = false;
	for (int i = 0; i < sizeArray; ++i) {
		Block* b = (Block*) arrayListOfBlocks->Get(i);
		if (b->getActive()) {
			v1x = b->getPosX();
			v1y = b->getPosY();

			v2x = b->getPosX() + b->getTextureWidth();
			v2y = b->getPosY() + b->getTextureHeight();
			passedFirst = false;
			result = 0;
			changed = false;
			needtoCheck = false;
			if (isAxesInsideASquare(x1Pos, v1x, v2x) ) {
				passedFirst = true;
				//check if both inside
				if (isAxesInsideASquare(x2Pos, v1x, v2x)) {
					changed = true;
					if (x1Pos - v1x > v2x - x2Pos) {
						xChoosed = x1Pos;
					}
					else {
						xChoosed = x2Pos;
					}
				}
				else {
					xChoosed = x1Pos;
				}
			}
			if (!changed && isAxesInsideASquare(x2Pos, v1x, v2x)) {
				passedFirst = true;
				xChoosed = x2Pos;
			}

			changed = false;

			if (isAxesInsideASquare(y1Pos, v1y, v2y)) {
				needtoCheck = true;
				//check if both inside
				if (isAxesInsideASquare(y2Pos, v1y, v2y)) {
					changed = true;
					if (y1Pos - v1y > v2y - y2Pos) {
						yChoosed = y1Pos;
					}
					else {
						yChoosed = y2Pos;
					}
				}
				else {
					yChoosed = y1Pos;
				}
			}
			if (!changed && isAxesInsideASquare(y2Pos, v1y, v2y)) {
				needtoCheck = true;
				yChoosed = y2Pos;
			}

			if (needtoCheck) {
				result = isPointInsideASquare(xChoosed, yChoosed, v1x, v2x, v1y, v2y, offsetYDetection);
			}
			if (result != 0) {
				b->Hit();
				if (result == -1) {

					if (getResetPos(y1Pos, y2Pos, v1y, v2y)) {
						objectToCheck->setPosY(v2y + correction);
					}
					else {
						objectToCheck->setPosY(v1y - objectToCheck->getTextureHeight() - correction);
					}
				}
				else{
					if (getResetPos(x1Pos, x2Pos, v1x, v2x)) {
						objectToCheck->setPosX(v2x + correction);
					}
					else {
						objectToCheck->setPosX(v1x - objectToCheck->getTextureWidth() - correction);
					}
				}
				return result;
			}
		}
	}
	return 0;
}

bool CollisionManager::CheckBallPosition(GameObject* objectToCheck, float x1Pos, float y1Pos, float x2Pos, float y2Pos) {
	int sizeArray = arrayOfBalls->GetSize();
	float v1x, v1y, v2x, v2y;
	float xChoosed = x1Pos;
	float yChoosed = y1Pos;
	int result = 0;
	bool changed = false;
	bool passedFirst = false;
	bool needtoCheck = false;
	for (int i = 0; i < sizeArray; ++i) {
		Ball* b = (Ball*) arrayOfBalls->Get(i);
		if (b->getActive()) {
			v1x = b->getPosX();
			v1y = b->getPosY();

			v2x = b->getPosX() + b->getTextureWidth();
			v2y = b->getPosY() + b->getTextureHeight();
			passedFirst = false;
			result = 0;
			changed = false;
			needtoCheck = false;
			if (isAxesInsideASquare(x1Pos, v1x, v2x)) {
				passedFirst = true;
				//check if both inside
				if (isAxesInsideASquare(x2Pos, v1x, v2x)) {
					changed = true;
					xChoosed = x1Pos + objectToCheck -> getTextureWidth()/2;
				}
				else {
					xChoosed = x1Pos;
				}
			}
			if (!changed && isAxesInsideASquare(x2Pos, v1x, v2x)) {
				passedFirst = true;
				xChoosed = x2Pos;
			}

			changed = false;

			if (isAxesInsideASquare(y1Pos, v1y, v2y)) {
				needtoCheck = true;
				//check if both inside
				if (isAxesInsideASquare(y2Pos, v1y, v2y)) {
					changed = true;
					yChoosed = objectToCheck->getTextureHeight() / 2; // i take the center to decide the type of bounce
				}
				else {
					yChoosed = y1Pos;
				}
			}
			if (!changed && isAxesInsideASquare(y2Pos, v1y, v2y)) {
				needtoCheck = true;
				yChoosed = y2Pos;
			}

			if (needtoCheck) {
				result = isPointInsideASquare(xChoosed, yChoosed, v1x, v2x, v1y, v2y, offsetYDetection);
			}
			if (result != 0) {
					if (getResetPos(x1Pos, x2Pos, v1x, v2x)) {
						objectToCheck->setPosX(v2x + correction);
					}
					else {
						objectToCheck->setPosX(v1x - objectToCheck->getTextureWidth() - correction);
					}
				return true;
			}
		}
	}
	return false;
}
/*
void CollisionManager::FinishedToAddElement() {
	int z = 0;
	int sizeArray = arrayOfBalls->GetSize();
	for (int i = 0; i < sizeArray; ++i) {
		GameObject* b = arrayOfBalls->Get(i);
		VERTEX* v = b->getVertices();
		int dimVertex = b->getDimVertecies();
		for (int j = 0; j < dimVertex; ++j, ++z) {
			allVertices[z] = v[j];
		}
	}
	sizeArray = arrayListOfBlocks->GetSize();
	for (int i = 0; i < sizeArray; ++i, ++z) {
		GameObject* b = arrayOfBalls->Get(i);
		VERTEX* v = b->getVertices();
		int dimVertex = b->getDimVertecies();
		for (int j = 0; j < dimVertex; ++j, ++z) {
			allVertices[z] = v[j];
		}
	}
	dimVertices = z;
	allVertices = new VERTEX[z];
}
int CollisionManager::GetDimVerticies()
{
	return dimVertices;
}
void* CollisionManager::GetAllVertex() {
	int sizeArray = arrayOfBalls->GetSize();
	int z = 0;
	for (int i=0; i < sizeArray; ++i) {
		GameObject * b = arrayOfBalls->Get(i);
		VERTEX* v = b->getVertices();
		int dimVertex = b->getDimVertecies();
		for (int j = 0; j < dimVertex; ++j,++z) {
			allVertices[z] = v[j];
		}
	}
	sizeArray = arrayListOfBlocks->GetSize();
	for (int i=0; i < sizeArray; ++i,++z) {
		GameObject* b = arrayOfBalls->Get(i);
		VERTEX* v = b->getVertices();
		int dimVertex = b->getDimVertecies();
		for (int j = 0; j < dimVertex; ++j, ++z) {
			allVertices[z] = v[j];
		}
	}
	return allVertices;
}*/