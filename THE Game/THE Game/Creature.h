#ifndef _Creature
#define _Creature

#include"Tiles.h"
#include"TerrainElements.h"
#include "Attack.h"
#include "Stats.h"
#include "Menu.h"


CMenu  menu;				// Create menu
enum monstersTypes
{
	Devil = 17,
	Wolf = 18,
	Orc = 19,
	Shade = 20,
	Skeleton = 21,
	Imp = 22,
	Mantikora = 23,
	Dragon = 24,
	TotalMonstersTypes = 8,
};

LTexture gMonsterTexture;
std::vector<SDL_Rect[TotalDirections][3]>monsterClips[TotalMonstersTypes];
Uint32 attackingTimer(Uint32 inter, void* par);
class CCreature :public LTexture, public  CBaseOfObjects, public CStats
{
protected:
	int type;
	bool ifAttacking = false;
	CAttack normalAttack;
	CAttack specialAttack;
	//position
	int posX, posY;
	int state = Down;
	int frame = 3;

	//Velocity
	int velX = 0;
	int velY = 0;
	int maxVel = 1;
	
	//animation
	int WALKING_ANIMATION_FRAMES = 3;
	SDL_Rect gSpriteClips[TotalDirections][3];
public:
	CCreature();
	CCreature(int _baseHp, int _baseDmg, int _lvl , int _hpLvlMultiplier, int DmgLvlMultiplier);
	CCreature(int _x, int _y,int _type);
	
	void loadSprites(int x, int y, int w, int h);
	void render(SDL_Rect& camera);
	void walkingAnimation();
	void motion(CTile* tiles[], std::vector<CTerrainElements> &terrain, std::vector<CCreature> &monsters, CCreature &_hero, int iterator = -1);
	bool touchesImpassibleTile(CTile* tiles[]);
	bool touchesImpassibeObjects(std::vector<CTerrainElements> &terrain);
	bool touchesMonsterOrHero(std::vector<CCreature> &monsters, CCreature &_hero, int iterator);
	void checkAndChangeState();
	bool ifMoving();
	void renderNormalAttack(SDL_Rect &camera);
	void attackDozer();
	void renderEnd(CCreature &_hero);

	void setXY(int _x, int _y);
	void setMaxVelocity(int _maxVelocity);
	void setWH(int _w, int _h);
	void setXYSpeed(int _xSpeed, int _ySpeed);
	void setAttack();
	void setFrame(int _newFrame);

	int getMaxVel();
	int getType();
	int getX(); 
	int getY();
};
CCreature::CCreature()
{
	posX = -1;
	posY = -1;
}
CCreature::CCreature(int _x, int _y,int _type)
{
	posX = _x;
	posY = _y;
	mCollider.x = _x;
	mCollider.y = _y;
	type = _type;
	setAttack();
}
void CCreature::setFrame(int _newFrame)
{
	frame = _newFrame;
}
void CCreature::setAttack()
{
	normalAttack.setAttack(mCollider.x, mCollider.y, type);
}
void CCreature::walkingAnimation()
{
	//Walking animation
		++frame;
		if (frame / 3 >= WALKING_ANIMATION_FRAMES)
			frame = 0;
}
void CCreature::motion(CTile* tiles[], std::vector<CTerrainElements> &terrain, std::vector<CCreature> &monsters, CCreature &_hero, int iterator)
{
	if (velX != 0)
	{
		//Move left or right
		posX += velX;
		mCollider.x = posX;
		if ((posX < 0) || (posX + mCollider.w >= LEVEL_WIDTH) || (touchesImpassibleTile(tiles)))
		{
			//Move back
			posX -= velX;
			mCollider.x = posX;
			velX = 0;
		}
		else if ((touchesImpassibeObjects(terrain)))
		{
			posX -= velX;
			mCollider.x = posX;
			velX = 0;
		}
		else if (touchesMonsterOrHero(monsters, _hero, iterator))
		{
			posX -= velX;
			mCollider.x = posX;
			velX = 0;
		}
	}
	//Move up or down
	{
		posY += velY;
		mCollider.y = posY;
		if ((posY < 0) || (posY + mCollider.h >= LEVEL_HEIGHT) || touchesImpassibleTile(tiles))
		{
			//Move back
			posY -= velY;
			mCollider.y = posY;
			velY = 0;
		}
		else if ((touchesImpassibeObjects(terrain)))
		{
			posY -= velY;
			mCollider.y = posY;
			velY = 0;
		}
		else if (touchesMonsterOrHero(monsters, _hero, iterator))
		{
			posY -= velY;
			mCollider.y = posY;
			velY = 0;
		}
	}
	if (ifMoving())
		walkingAnimation();
}
bool CCreature::touchesMonsterOrHero(std::vector<CCreature> &monsters, CCreature &_hero, int iterator)
{
	//go throught monsters
	if (iterator == -1) // for hero
	{
		for (int i = 0; i < monsters.size(); i++)
		{
			if (checkCollision(mCollider, monsters[i].getMCollider()))
				return true;
		}
	}
	else // for monsters
	{
		SDL_Rect tempC = _hero.getMCollider();
		if (checkCollision(mCollider, tempC))
		{
			if (ifAttacking == false) // attacking hero
			{
				normalAttack.renderingAttackOnOff();
				attackDozer();	
				if (_hero.dealDmg(getTotalDmg()) == -1)
					renderEnd(_hero);
			}
			return true;
		}			
		for (int i = 0; i < monsters.size(); i++)
		{
			if (checkCollision(mCollider, monsters[i].getMCollider()) && i != iterator) // exception is colliding with itself
				return true;
		}
	}
	return false;
}

bool CCreature::touchesImpassibeObjects(std::vector<CTerrainElements> &terrain)
{
	//go throught objects
	for (int i = 0; i < terrain.size(); i++)
	{
		//check if collides
		for (int j = 0; j < terrain[i].getCollidersNumber(); j++)
		{
			if (checkCollision(mCollider, terrain[i].getSpecificCollider(j)))
				return true;
		}
	}
	//else
	return false;
}
void CCreature::render(SDL_Rect& camera)
{
	if (checkCollision(camera))		//check if on screen
	{		
		//Show the element	
		SDL_Rect* gCurrentClip = &gSpriteClips[state][frame/3];
		gMonsterTexture.renderTextureWithSize(posX - camera.x, posY - camera.y, mCollider.w, mCollider.h, gCurrentClip);
	}
}
void CCreature::renderEnd(CCreature &_hero) // Rendering finishing animation and exiting the game; 
{
	menu.soundEffect(Death);
	menu.startPauseMusic(0);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(gRenderer);
	SDL_Color textColor = { 255, 0, 0 };
	LTexture endText;
	endText.loadFromRenderedText("A", textColor,iconFont);
	endText.renderTexture((SCREEN_WIDTH - endText.getWidth()) / 2+20, (SCREEN_HEIGHT - endText.getHeight()) / 2 -150);
	SDL_RenderPresent(gRenderer);
	menu.saveScoreboard(_hero.calculateEXP());
	SDL_Delay(1500);
	endText.loadFromRenderedText("GAME OVER", textColor,menuFont);
	endText.renderTexture((SCREEN_WIDTH-endText.getWidth())/2,(SCREEN_HEIGHT-endText.getHeight())/2+100);
	SDL_RenderPresent(gRenderer);
	SDL_Delay(3000);
	exit(0);
}
void CCreature::loadSprites(int x, int y, int w, int h)
{
	//Set sprite clips 
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			gSpriteClips[j][i].x = x + w * i;
			gSpriteClips[j][i].y = y + h * j;
			gSpriteClips[j][i].w = w;
			gSpriteClips[j][i].h = h;
		}
	}
}
bool CCreature::touchesImpassibleTile(CTile* tiles[])
{
	//Go through the tiles
	for (int i = 0; i < TOTAL_TILES; i++)
	{
		//If the tile is an impassible terrain
		if ((tiles[i]->getType() == waterBrown) || (tiles[i]->getType() == waterGreen))
		{
			if (checkCollision(mCollider, tiles[i]->getMCollider()))
			{
				return true;
			}
		}
	}
	//else
	return false;
}
void CCreature::setMaxVelocity(int _maxVelocity)
{
	maxVel = _maxVelocity;
}
void CCreature::setXY(int _x, int _y)
{
	posX = _x;
	posY = _y;
	mCollider.x = _x;
	mCollider.y = _y;
}
void CCreature::setWH(int _w, int _h)
{
	mCollider.w = _w;
	mCollider.h = _h;
}
int CCreature::getMaxVel()
{
	return maxVel;
}
int CCreature::getType()
{
	return type;
}
int CCreature::getX()
{
	return mCollider.x;
}
int CCreature::getY()
{
	return mCollider.y;
}
void CCreature::setXYSpeed(int _xSpeed, int _ySpeed)
{
	velX = _xSpeed;
	velY = _ySpeed;
}
void CCreature::checkAndChangeState()
{
	if (velX > 0 && velY == 0)
		state = Right;
	else if (velX < 0 && velY == 0)
		state = Left;
	else if (velY > 0)
		state = Down;
	else if (velY < 0)
		state = Up;
}
bool CCreature::ifMoving()
{
	if (velX != 0 || velY != 0)
		return true;
	return false;
}
void CCreature::renderNormalAttack(SDL_Rect &camera)
{
	normalAttack.renderAttack(camera, mCollider.x, mCollider.y, state);
}
void CCreature::attackDozer()
{
	if (type == -1)
		menu.soundEffect(Punch);
	else
		menu.soundEffect(Hit);
	ifAttacking = true;
	SDL_TimerID timer = SDL_AddTimer(getAttackSpeed(), renderOFF, &ifAttacking);
}
Uint32 attackingTimer(Uint32 inter, void* par)
{
	bool *newPar = static_cast<bool*>(par);
	*newPar = false;
	return 0;
}

#endif