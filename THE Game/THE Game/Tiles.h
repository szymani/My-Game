#ifndef _Tiles
#define _Tiles

#include"ObjectsBase.h"

//Tile constants
const int TILE_WIDTH = 50;
const int TILE_HEIGHT = 50;
const int TOTAL_TILES = 1350;

enum TileTypes
{
waterGreen,
dirt,
grass,
white,
grass2,
waterBrown,
totalTileKinds
};

LTexture gTileTexture;
SDL_Rect gTileClips[totalTileKinds];

class CTile :public  CBaseOfObjects
{
public:
	CTile();
	//Initializes position and type
	CTile(int x, int y, int tileType);

	//Shows the tile
	void render(SDL_Rect& camera);

	//Get the tile type
	int getType();
private:
	//The tile type
	int mType;
};
CTile::CTile()
{
	//Set the collision box
	mCollider.x = 0;
	mCollider.y = 0;
	mCollider.w = 0;
	mCollider.h = 0;

	//Get the tile type
	mType = 0;
}
CTile::CTile(int x, int y, int tileType)
{
	//Set the collision box
	mCollider.x = x;
	mCollider.y = y;
	mCollider.w = TILE_WIDTH;
	mCollider.h = TILE_HEIGHT;

	//Get the tile type
	mType = tileType;
}
void CTile::render(SDL_Rect& camera)
{
	//If the tile is on screen
	if (checkCollision(camera, mCollider))
	{
		//Show the tile
		gTileTexture.renderTexture(mCollider.x - camera.x, mCollider.y - camera.y, &gTileClips[mType]);
	}
}
int CTile::getType()
{
	return mType;
}

#endif