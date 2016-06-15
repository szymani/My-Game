#ifndef _Terrain
#define _Terrain

#include"ObjectsBase.h"
#include<vector>

enum TerrainTypes
{
Nothing,
Portal,
Gate,
Cave,
Sculpture,
Altar,
BigTomb,
Well,
Fountain,
SmallTomb,
CrossTomb,
TreeWall,
TreeGroup,
SingleBigTree,
Bush,
FallenTree,
SingleSmallTree,
TotalElementTypes
};

LTexture gElementTexture;
SDL_Rect terrainElementsClips[TotalElementTypes];
class CTerrainElements :public LTexture ,public CBaseOfObjects			
{
protected:
	int elementsType;
	bool onTop = false;
	std::vector<SDL_Rect> colliders;
	LTexture gTexture;
public:
	CTerrainElements(){}
	CTerrainElements(int x, int y, int type);
	void render(SDL_Rect& camera);

	std::vector<SDL_Rect> getColliders();
	void addCollider(int _x, int _y, int _w, int _h);
	void setAllColliders(std::vector<SDL_Rect> &_colliders);
	void addCollider(SDL_Rect _collider);

	SDL_Rect getSpecificCollider(int _number);
	int getCollidersNumber();
	bool isOnTop();
};
void CTerrainElements::addCollider(int _x, int _y, int _w, int _h)
{
	SDL_Rect tempC;
	tempC.x = _x;
	tempC.y = _y;
	tempC.w = _w;
	tempC.h = _h;
	colliders.push_back(tempC);
}
CTerrainElements::CTerrainElements(int x, int y, int type)
{
	//set position box
	mCollider.x = x;
	mCollider.y = y;
	elementsType = type;
	//set collision boxes
	switch (type)
	{
	case Portal: mCollider.x = x - 25; mCollider.w = 100; mCollider.h = 100; addCollider(mCollider);	break;
	case Gate:
	{
		onTop = true;
		mCollider.x = x - 25;	mCollider.y = y - 25;
		mCollider.w = 100;		mCollider.h = 100;
		addCollider(mCollider.x, mCollider.y, 25, 100);
		addCollider(mCollider.x + 75, mCollider.y, 25, 100);
		break;
	}
	case Cave:
	{
		addCollider(mCollider.x, mCollider.y, 250, 70);
		addCollider(mCollider.x+20, mCollider.y+70, 80, 50);
		addCollider(mCollider.x + 180, mCollider.y + 70, 100, 30);
		addCollider(mCollider.x + 190, mCollider.y + 100, 60, 30);
		break;
	}
	case Sculpture: mCollider.w = 50; mCollider.h = 100; addCollider(mCollider);	break;
	case SingleBigTree:
	{
		onTop = true;
		mCollider.w = 160;		mCollider.h = 150;
		addCollider(mCollider.x+70, mCollider.y+100, 30, 50);
		break;
	}
	case SmallTomb:	mCollider.x = x +12; mCollider.y = y + 12;mCollider.w = 20; mCollider.h = 20; addCollider(mCollider);	break;
	case CrossTomb:	mCollider.x = x + 12; mCollider.y = y + 12; mCollider.w = 20; mCollider.h = 20; addCollider(mCollider);	break;

	}
}

void CTerrainElements::render(SDL_Rect& camera)
{
	//check if on screen
	if (checkCollision(camera))
	{	
		//Show the element
		gElementTexture.renderTexture(mCollider.x - camera.x, mCollider.y - camera.y, &terrainElementsClips[elementsType]);
	}
}
std::vector<SDL_Rect> CTerrainElements::getColliders()	
{
	return colliders;
}
void CTerrainElements::setAllColliders(std::vector<SDL_Rect> &_colliders)
{
	colliders = _colliders;
}
void CTerrainElements::addCollider(SDL_Rect _collider)
{
	colliders.push_back(_collider);
}
SDL_Rect CTerrainElements::getSpecificCollider(int _number)
{
	if (colliders.size() != 0)
	{
		return colliders[_number];
	}
	else
		return mCollider;
}
int CTerrainElements::getCollidersNumber()
{
	return colliders.size();
}
bool CTerrainElements::isOnTop()
{
	return onTop;
}

#endif