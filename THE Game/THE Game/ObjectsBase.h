#ifndef _Base
#define _Base

#include"LTexture.h"

class CBaseOfObjects
{
protected:
	SDL_Rect mCollider;
public:
	CBaseOfObjects(){}
	CBaseOfObjects(int x, int y)
	{
		mCollider.x = x;
		mCollider.y = y;
	}
	bool checkCollision(SDL_Rect b);
	bool checkCollision(SDL_Rect a, SDL_Rect b);

	void setCollider(int _x, int _y, int _h, int _w);
	SDL_Rect getMCollider();

};

bool  CBaseOfObjects::checkCollision(SDL_Rect b)
{
	SDL_Rect a = mCollider;
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
		return false;
	if (topA >= bottomB)
		return false;
	if (rightA <= leftB)
		return false;
	if (leftA >= rightB)
		return false;

	//If none of the sides from A are outside B
	return true;
}
bool  CBaseOfObjects::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
		return false;
	if (topA >= bottomB)
		return false;
	if (rightA <= leftB)
		return false;
	if (leftA >= rightB)
		return false;

	//If none of the sides from A are outside B
	return true;
}
void CBaseOfObjects::setCollider(int _x, int _y, int _h, int _w)
{
	mCollider.x = _x;
	mCollider.y = _y;
	mCollider.w = _w;
	mCollider.h = _h;
}
SDL_Rect CBaseOfObjects::getMCollider()
{
	return mCollider;
}

#endif