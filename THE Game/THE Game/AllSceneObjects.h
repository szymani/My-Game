#ifndef _Scene
#define _Scene

#include"Hero.h"
#include"Monsters.h"

// Wrapper of all objects on scene, its not universal,just for this project(setting tiles,
class  CAllSceneObjects
{
protected:
	CHero hero;
	CMonsters allMonsters;
	std::vector<CTerrainElements> terrainObjects;
	CTile* tiles[TOTAL_TILES];
	SDL_Rect camera;
public:
	//Initiation
	CAllSceneObjects(){}
	bool loadHeroTexture(std::string name);
	bool loadMonstersTexture(std::string name);
	bool setMonsters();
	void setMonstersClips();
	bool setTerrainElements();
	void setTerrainElementsClips();
	void createCamera();

	//Setters
	void setTileClips();
	bool setTiles();
	void settingHero(int choice);

	//Getters
	CTile** getTiles();

	//Rest
	void eventsHandling(SDL_Event &e);
	void renderAll();
	void movement();
};
bool CAllSceneObjects::loadHeroTexture(std::string name)
{
	return hero.loadFromFile(name);
}
void CAllSceneObjects::settingHero(int choice)
{
	hero.setHeroStats(choice);
	if (choice == 0)
		hero.loadSprites(0, 0, 50, 50);
	else if (choice == 1)
		hero.loadSprites(150, 0, 50, 50);
			
}
bool CAllSceneObjects::loadMonstersTexture(std::string name)
{
	return gMonsterTexture.loadFromFile(name);
}
bool CAllSceneObjects::setMonsters()
{
	//Success flag
	bool monstersLoaded = true;
	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("terrain.map");

	//If the map couldn't be loaded
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		monstersLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int monstersType = -1;

			//Read tile from map file
			map >> monstersType;
			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				monstersLoaded = false;
				break;
			}
			//If the number is a valid type number
			if ((monstersType >= TotalElementTypes) && (monstersType < TotalElementTypes + TotalMonstersTypes))
			{
				//adding monster that already have stats and clips included
				allMonsters.addMonster(x, y, monstersType);
			}
			//Move to next tile spot
			x += TILE_WIDTH;
			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;
				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
	}
	//Close the file
	map.close();

	//If the map was loaded fine
	return monstersLoaded;
}
void CAllSceneObjects::createCamera()
{
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

CTile** CAllSceneObjects::getTiles()
{
	return tiles;
}
void CAllSceneObjects::setTileClips()
{
	gTileClips[waterGreen].x = 0;
	gTileClips[waterGreen].y = 0;
	gTileClips[waterGreen].w = TILE_WIDTH;
	gTileClips[waterGreen].h = TILE_HEIGHT;

	gTileClips[dirt].x = 50;
	gTileClips[dirt].y = 0;
	gTileClips[dirt].w = TILE_WIDTH;
	gTileClips[dirt].h = TILE_HEIGHT;

	gTileClips[grass].x = 100;
	gTileClips[grass].y = 0;
	gTileClips[grass].w = TILE_WIDTH;
	gTileClips[grass].h = TILE_HEIGHT;

	gTileClips[white].x = 0;
	gTileClips[white].y = 50;
	gTileClips[white].w = TILE_WIDTH;
	gTileClips[white].h = TILE_HEIGHT;

	gTileClips[grass2].x = 50;
	gTileClips[grass2].y = 50;
	gTileClips[grass2].w = TILE_WIDTH;
	gTileClips[grass2].h = TILE_HEIGHT;

	gTileClips[waterBrown].x = 100;
	gTileClips[waterBrown].y = 50;
	gTileClips[waterBrown].w = TILE_WIDTH;
	gTileClips[waterBrown].h = TILE_HEIGHT;
}
bool CAllSceneObjects::setTiles()
{
	//Success flag
	bool success = true;
	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("background.map");
	//If the map couldn't be loaded
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		success = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;
			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				success = false;
				break;
			}
			//If the number is a valid tile number
			if ((tileType >= 0) && (tileType < totalTileKinds))
			{
				tiles[i] = new CTile(x, y, tileType);
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf("Error loading map: Invalid tile type at %d!\n", i);
				success = false;
				break;
			}
			//Move to next tile spot
			x += TILE_WIDTH;
			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;
				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
		//Clip the sprite sheet
		if (success)
		{
			setTileClips();
		}
	}
	//Close the file
	map.close();

	//If the map was loaded fine
	return success;
}
bool CAllSceneObjects::setTerrainElements()
{
	//Success flag
	bool terrainLoaded = true;
	//The tile offsets
	int x = 0, y = 0;

	//Open the map
	std::ifstream map("terrain.map");

	//If the map couldn't be loaded
	if (!map.is_open())
	{
		printf("Unable to load map file!\n");
		terrainLoaded = false;
	}
	else
	{
		//Initialize the tiles
		for (int i = 0; i < TOTAL_TILES; ++i)
		{
			//Determines what kind of tile will be made
			int elementsType = -1;

			//Read tile from map file
			map >> elementsType;
			//If the was a problem in reading the map
			if (map.fail())
			{
				//Stop loading map
				printf("Error loading map: Unexpected end of file!\n");
				terrainLoaded = false;
				break;
			}
			//If the number is a valid type number
			if ((elementsType > 0) && (elementsType < TotalElementTypes))
			{
				CTerrainElements temp(x, y, elementsType);
				terrainObjects.push_back(temp);
			}
			//If we don't recognize the type
			//else if (elementsType!=0)
			//{
			//	printf("Error loading map: Invalid tile type at %d!\n", i);
			//	terrainLoaded = false;
			//	break;
			//}
			//Move to next tile spot
			x += TILE_WIDTH;
			//If we've gone too far
			if (x >= LEVEL_WIDTH)
			{
				//Move back
				x = 0;
				//Move to the next row
				y += TILE_HEIGHT;
			}
		}
		//Clip the sprite sheet
		if (terrainLoaded)
		{
			setTerrainElementsClips();
		}
	}
	//Close the file
	map.close();

	//If the map was loaded fine
	return terrainLoaded;
}
void CAllSceneObjects::setTerrainElementsClips()
{
	terrainElementsClips[Portal].x = 0;
	terrainElementsClips[Portal].y = 0;
	terrainElementsClips[Portal].w = 100;
	terrainElementsClips[Portal].h = 100;

	terrainElementsClips[Gate].x = 100;
	terrainElementsClips[Gate].y = 0;
	terrainElementsClips[Gate].w = 100;
	terrainElementsClips[Gate].h = 100;

	terrainElementsClips[Cave].x = 200;
	terrainElementsClips[Cave].y = 0;
	terrainElementsClips[Cave].w = 300;
	terrainElementsClips[Cave].h = 170;

	terrainElementsClips[Sculpture].x = 0;
	terrainElementsClips[Sculpture].y = 100;
	terrainElementsClips[Sculpture].w = 50;
	terrainElementsClips[Sculpture].h = 100;

	terrainElementsClips[SingleBigTree].x = 0;
	terrainElementsClips[SingleBigTree].y = 210;
	terrainElementsClips[SingleBigTree].w = 160;
	terrainElementsClips[SingleBigTree].h = 150;

	terrainElementsClips[SmallTomb].x = 200;
	terrainElementsClips[SmallTomb].y = 170;
	terrainElementsClips[SmallTomb].w = 30;
	terrainElementsClips[SmallTomb].h = 30;

	terrainElementsClips[CrossTomb].x = 230;
	terrainElementsClips[CrossTomb].y = 170;
	terrainElementsClips[CrossTomb].w = 30;
	terrainElementsClips[CrossTomb].h = 30;
}

void CAllSceneObjects::eventsHandling(SDL_Event &e)
{
	int iterator = hero.heroEventsHandling(e, camera, allMonsters.getMonstersVector());
	if (iterator != -1)								// if hero hit enemy
	{
		int exp = allMonsters.checkDMG(iterator, hero.getTotalDmg());
		if ( exp == -1)				//if the dragon is killed
		{
			menu.saveScoreboard(hero.calculateEXP());
			SDL_Delay(3000);
			exit(0);
		}
		else
			hero.addExp(exp);		//deal dmg, check if dead and add exp
	}

}
void CAllSceneObjects::renderAll()
{
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	//rendering tiles
	for (int i = 0; i < TOTAL_TILES; ++i)
	{
		tiles[i]->render(camera);
	}
	//rendering background terrain elements
	for (int i = 0; i < terrainObjects.size(); ++i)
	{
		if (!terrainObjects[i].isOnTop())
			terrainObjects[i].render(camera);
	}
	//rendering creatures
	allMonsters.renderMonsters(camera);
	hero.render(camera);
	allMonsters.renderAttacks(camera);
	//rendering front terrain elements
	for (int i = 0; i < terrainObjects.size(); ++i)
	{
		if (terrainObjects[i].isOnTop())
		terrainObjects[i].render(camera);
	}
}
void CAllSceneObjects::movement()
{
	hero.motion(tiles, terrainObjects, allMonsters.getMonstersVector(), hero);
	hero.setCamera(camera);
	allMonsters.moveMonsters(tiles, terrainObjects,hero);
	allMonsters.checkIfClose(hero);
}

#endif