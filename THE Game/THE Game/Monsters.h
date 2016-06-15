#ifndef _Monsters
#define _Monsters

#include"Creature.h"
#include<math.h>

class CMonsters
{
protected:
	int monstersNumber = 0;
	std::vector<CCreature> monsters;
public:
	CMonsters(){}
	void setMonsters();
	void addMonster(int _x, int _y, int _type);
	int getMonstersNumber();
	void renderMonsters(SDL_Rect& camera);
	std::vector<CCreature> getMonstersVector();
	void checkIfClose(CHero &_hero);
	void moveMonsters(CTile* tiles[], std::vector<CTerrainElements> &terrain,CHero &_hero);
	void renderAttacks(SDL_Rect& camera);
	int checkDMG(int iterator, int attackersDmg);
	bool ifWon();

	//setting clips and stats
	void setMonsters(CCreature &temp);
	void setDevil(CCreature &temp);
	void setWolf(CCreature &temp);
	void setOrc(CCreature &temp);
	void setShade(CCreature &temp);
	void setSkeleton(CCreature &temp);
	void setImp(CCreature &temp);
	void setMantikora(CCreature &temp);
	void setDragon(CCreature &temp);
};
bool CMonsters::ifWon()
{
	for (int i = 0; i < monsters.size(); i++)
	{
		if (monsters[i].getType() == 24)
			return false; 
	}
	return true;
}
int CMonsters::checkDMG(int iterator, int attackersDmg)					// return 0 if monster not killed , -1 if fragon slain, exp drop if monster killed
{
	if (iterator >= 0 && iterator < monsters.size())
	{
		int temp = monsters[iterator].dealDmg(attackersDmg);				// return expDrop if killed, else return 0
		if (temp != 0)
		{   
  			monsters.erase(monsters.begin()+iterator);
			if (ifWon())													//if the dragon is killed
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
				SDL_RenderClear(gRenderer);
				SDL_Color textColor = { 255, 0, 0 };
				LTexture endText;
				endText.loadFromRenderedText("YOU WON", textColor, menuFont);
				endText.renderTexture((SCREEN_WIDTH - endText.getWidth()) / 2, (SCREEN_HEIGHT - endText.getHeight()) / 2);
				SDL_RenderPresent(gRenderer);
				return -1;
			}
			return temp;
		}			
		return 0;
	} 		
	return 0;
}
void CMonsters::renderAttacks(SDL_Rect& camera)
{
	for (int i = 0; i < monsters.size(); i++)
		monsters[i].renderNormalAttack(camera);
}
void CMonsters::moveMonsters(CTile* tiles[], std::vector<CTerrainElements> &terrain, CHero &_hero)
{
	for (int i = 0; i < monsters.size(); i++)
		monsters[i].motion(tiles, terrain,monsters,_hero,i);
}
void CMonsters::checkIfClose(CHero &_hero)
{
	for (int i = 0; i < monsters.size(); i++)
	{
		//set monsters velocity in hero's direction
		if (sqrt(pow(_hero.getMCollider().x - monsters[i].getMCollider().x, 2) + pow(_hero.getMCollider().y - monsters[i].getMCollider().y, 2)) <= 250)
		{
			if (_hero.getMCollider().x>monsters[i].getMCollider().x)
			{
				if (_hero.getMCollider().y > monsters[i].getMCollider().y)
					monsters[i].setXYSpeed(monsters[i].getMaxVel(), monsters[i].getMaxVel());
				else if (_hero.getMCollider().y < monsters[i].getMCollider().y)
					monsters[i].setXYSpeed(monsters[i].getMaxVel(), -monsters[i].getMaxVel());
				else
					monsters[i].setXYSpeed(monsters[i].getMaxVel(), 0);
			}
			else if (_hero.getMCollider().x < monsters[i].getMCollider().x)
			{
				if (_hero.getMCollider().y > monsters[i].getMCollider().y)
					monsters[i].setXYSpeed(-monsters[i].getMaxVel(), monsters[i].getMaxVel());
				else if (_hero.getMCollider().y < monsters[i].getMCollider().y)
					monsters[i].setXYSpeed(-monsters[i].getMaxVel(), -monsters[i].getMaxVel());
				else
					monsters[i].setXYSpeed(-monsters[i].getMaxVel(), 0);
			}
			else
			{
				if (_hero.getMCollider().y > monsters[i].getMCollider().y)
					monsters[i].setXYSpeed(0, monsters[i].getMaxVel());
				else
					monsters[i].setXYSpeed(0, -monsters[i].getMaxVel());
			}
			monsters[i].checkAndChangeState();
		}
		else
		{
			monsters[i].setXYSpeed(0, 0);
			monsters[i].setFrame(3);
		}
	}
}
void CMonsters::renderMonsters(SDL_Rect& camera)
{
	for (int i = 0; i < monsters.size(); i++)
		monsters[i].render(camera);
}
int CMonsters::getMonstersNumber()
{
	return monstersNumber;
}
std::vector<CCreature> CMonsters::getMonstersVector()
{
	return monsters;
}
void  CMonsters::addMonster(int _x, int _y, int _type)
{
	CCreature tempC(_x, _y, _type);
	setMonsters(tempC);
	monsters.push_back(tempC);
	monstersNumber++;
}
void CMonsters::setMonsters(CCreature &temp)
{
	switch (temp.getType())
	{	
	case Devil:		setDevil(temp); 		break;
	case Wolf:		setWolf(temp);			break;
	case Orc:		setOrc(temp);			break;
	case Shade:		setShade(temp);			break;
	case Skeleton:	setSkeleton(temp);		break;
	case Imp:		setImp(temp);			break;
	case Mantikora:	setMantikora(temp);		break;
	case Dragon:	setDragon(temp);		break;
	}
}

void CMonsters::setDevil(CCreature &temp)
{
	temp.setWH(50,50);
	temp.loadSprites(0, 0, 32, 32);
	temp.setStats(100, 20, 1, 1.5, 1.5);

}
void CMonsters::setWolf(CCreature &temp)
{
	temp.setWH(50, 50);
	temp.loadSprites(94, 0, 32, 32);
	temp.setStats(50, 10, 1, 1.5, 1.5,700);
	temp.setExpDrop(40);
}
void CMonsters::setOrc(CCreature &temp)
{
	temp.setWH(50, 50);
	temp.loadSprites(190, 0, 32, 32);
	temp.setStats(200, 40, 1, 1.5, 1.5);
}
void CMonsters::setShade(CCreature &temp)
{
	temp.setWH(50, 50);
	temp.loadSprites(285, 0, 32, 32);
	temp.setStats(70, 30, 1, 1.5, 1.5);
	temp.setExpDrop(40);
}
void CMonsters::setSkeleton(CCreature &temp)
{
	temp.setWH(50, 50);
	temp.loadSprites(381, 0, 32, 32);
	temp.setStats(40, 10, 1, 1.5, 1.5);
	temp.setExpDrop(30);
}
void CMonsters::setImp(CCreature &temp)
{
	temp.setWH(50, 50);
	temp.loadSprites(477, 0, 32, 32);
	temp.setStats(40, 10, 1, 1.5, 1.5);
	temp.setExpDrop(35);
}
void CMonsters::setMantikora(CCreature &temp)
{
	temp.setWH(100,100);
	temp.loadSprites(0, 128, 60, 60);
	temp.setStats(500, 80, 1, 1.5, 1.5,1200);
	temp.setExpDrop(100);
}
void CMonsters::setDragon(CCreature &temp)
{
	temp.setWH(130, 100);
	temp.loadSprites(180, 128, 80, 64);
	temp.setStats(1500, 200, 1, 1.5, 1.5,1500);
	temp.setExpDrop(200);
}

#endif