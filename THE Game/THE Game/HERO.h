#ifndef _HER0
#define _HER0

#include"Creature.h"
#include<string>

class CHero :public CCreature
{
protected:
	int specialAttack = -1;
	LTexture infoText;
public:
	CHero();
	void render(SDL_Rect& camera);
	void setCamera(SDL_Rect& camera);
	int heroEventsHandling(SDL_Event& e, SDL_Rect &camera,std::vector<CCreature> &monsters);
	void setHeroStats(int choice);
	int checkMonsters(std::vector<CCreature> &monsters);
	void renderInfoText();
};
CHero::CHero()
{
	type = -1;
}
void CHero::setHeroStats(int choice)
{
	ifHero = true;
	setLvl(1);
	setXY(50, 150);
	setMaxVelocity(2);
	setCollider(50, 50, 40, 40);
	if (choice == 0)
	{
		setStats(100, 40, 0, 40,25,40);
		specialAttack = -2;
	}	
	else if (choice == 1)
		setStats(160, 20, 0, 60, 15, 600);

	setAttack();
}
void CHero::render(SDL_Rect& camera)
{
	SDL_Rect* gCurrentClip = &gSpriteClips[state][frame / 3];
	renderTexture(posX - camera.x, posY - camera.y, gCurrentClip);
	normalAttack.renderAttack(camera, mCollider.x, mCollider.y, state);
	renderInfoText();
}
void CHero::setCamera(SDL_Rect& camera)
{
	//Center the camera over hero
	camera.x = (mCollider.x + mCollider.w / 2) - SCREEN_WIDTH / 2;
	camera.y = (mCollider.y + mCollider.h / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > LEVEL_WIDTH - camera.w)
		camera.x = LEVEL_WIDTH - camera.w;
	if (camera.y > LEVEL_HEIGHT - camera.h)
		camera.y = LEVEL_HEIGHT - camera.h;
}
int CHero::heroEventsHandling(SDL_Event& e, SDL_Rect &camera, std::vector<CCreature> &monsters)
{
	if (e.type == SDL_KEYDOWN)
	{		
		if (e.key.repeat == 0)
		{			
			switch (e.key.keysym.sym)
			{
			case SDLK_SPACE:
			{
				return checkMonsters(monsters);
				break;
			}
			case SDLK_UP:		velY -= maxVel;					 break;
			case SDLK_DOWN:		velY += maxVel;					 break;
			case SDLK_LEFT:		velX -= maxVel;					 break;
			case SDLK_RIGHT:	velX += maxVel;					 break;
			}		
			checkAndChangeState();
			return -1;		
		}				
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:		velY = 0; break;
		case SDLK_DOWN:		velY = 0; break;
		case SDLK_LEFT:		velX = 0; break;
		case SDLK_RIGHT:	velX = 0; break;
		}
		frame = 3;
		return -1;	
	}	
	else return -1;
}
int CHero::checkMonsters(std::vector<CCreature> &monsters)
{
	if (ifAttacking == false) // attacking monsters
	{
		normalAttack.renderingAttackOnOff();
		attackDozer();
		for (int i = 0; i < monsters.size(); i++)
		{
			if (checkCollision(normalAttack.getMCollider(), monsters[i].getMCollider()) && normalAttack.ifVisible() == true)
				return i;		
		}		
	}
	return -1;
}
void CHero::renderInfoText()
{
	SDL_Color textColor = { 255, 0, 0 };
	std::string tempText = "HP " + std::to_string(totalHp);
	infoText.loadFromRenderedText(tempText, textColor,gFont);
	infoText.renderTexture(0, 700);

	tempText = "DMG " + std::to_string(totalDmg);
	infoText.loadFromRenderedText(tempText, textColor,gFont);
	infoText.renderTexture(0, 728);

	tempText = "Lvl  " + std::to_string(lvl);
	infoText.loadFromRenderedText(tempText, textColor,gFont);
	infoText.renderTexture(0, 756);
}

#endif