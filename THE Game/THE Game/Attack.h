#ifndef _Attack
#define _Attack

#include "ObjectsBase.h"

enum movingDirections
{
	Down,
	Left,
	Right,
	Up,
	TotalDirections
};
Uint32 renderOFF(Uint32 inter, void* par);
LTexture gAttackTexture;
class CAttack:public CBaseOfObjects
{
protected:
	bool visible = false;
	int creatureType;
	int attackState;
	SDL_Rect attackSprite[TotalDirections];
public:
	CAttack(){}
	CAttack(int x, int y, int attackType);
	void renderAttack(SDL_Rect& camera, int x, int y,int state);
	void renderingAttackOnOff();
	void attackAnimation();
	void loadAttackSprites();
	
	void setAttackXY(int x,int y);
	void setAttack(int x, int y, int _creatureType);
	void setAttackState(int _state);	
	void setVisibility(bool choice);
	
	bool ifVisible();
};

CAttack::CAttack(int x, int y, int _creatureType)
{
	mCollider.x = x;
	mCollider.y = y;
	creatureType = _creatureType;
	if (creatureType != -1){mCollider.w = 50;	mCollider.h = 50;}
	attackState = Down;
}
void CAttack::setAttack(int x, int y, int _creatureType)
{

	creatureType = _creatureType;
	mCollider.w = 50; mCollider.h = 50;
	attackState = Down;
	loadAttackSprites();
}
void CAttack::renderAttack(SDL_Rect& camera,int x, int y,int state)
{
	if (visible == true )				// Show if active
	{
		attackState = state;				//setting new coordinates
		if (attackState == Down){		mCollider.y = y+30;		mCollider.x = x;		}
		else if (attackState == Up){	mCollider.y = y-30;		mCollider.x = x;		}
		else if (attackState == Left){	mCollider.y = y;		mCollider.x = x-30;		}
		else if (attackState == Right){ mCollider.y = y;		mCollider.x = x+30;		}
		SDL_Rect* gCurrentClip = &attackSprite[attackState];
		gAttackTexture.renderTexture(mCollider.x - camera.x, mCollider.y - camera.y, gCurrentClip);
	}
}
void CAttack::loadAttackSprites()
{
	int w = 50;	int h = 50;	int x = 0; int y = 0;
	if (creatureType != -1 && creatureType != 23 && creatureType != 24)
		x = 200;
	else if (creatureType == 23 || creatureType == 24)
	{
		y = 50;	w = 100; h = 100;
	}	
	for (int i = 0; i < TotalDirections; i++)
	{
		attackSprite[i].x = x + w * i;
		attackSprite[i].y = y;
		attackSprite[i].w = w;
		attackSprite[i].h = h;
	}
}
void CAttack::setAttackXY(int x, int y)
{
	mCollider.x = x;
	mCollider.y = y;
}
void CAttack::setAttackState(int _state)
{
	attackState = _state;
}
void CAttack::setVisibility(bool choice)
{
	visible = choice;
}
void CAttack::renderingAttackOnOff() 
{
	visible = true;
	SDL_TimerID timer = SDL_AddTimer(150, renderOFF, &visible);
}
Uint32 renderOFF(Uint32 inter, void* par)
{
	bool *newPar = static_cast<bool*>(par);
	*newPar = false;
	return 0;
}
bool CAttack::ifVisible()
{
	return visible;
}

#endif 