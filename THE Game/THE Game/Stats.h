#ifndef  _Stats
#define _Stats

class CStats
{
protected:
	int baseHp;
	int baseDmg;
	int lvl;
	float hpLvlMultiplier;
	float DmgLvlMultiplier;
	int attackSpeed = 1000;
	int totalHp;
	int totalDmg;
	int bonusHp = 0;
	int bonusDmg = 0;
	int expDrop = 50;
	int totalExp;
	bool ifHero = false;
public:
	CStats(){}
	int  dealDmg(int attackersDMG);
	void update();

	void setStats(int _baseHp, int _baseDmg, int _lvl, float _hpLvlMultiplier, float _DmgLvlMultiplier, int _attackSpeed = 1000);
	void setLvl(int _lvl);
	void setExpDrop(int _exp);
	void addExp(int _exp);

	int getAttackSpeed();
	CStats getStats();
	int getTotalDmg();
	int getTotalHp();
	int calculateEXP();
};
void CStats::update()
{
	totalHp = baseHp + lvl*hpLvlMultiplier + bonusHp;
	totalDmg = baseDmg + lvl*DmgLvlMultiplier + bonusDmg;
}
int CStats::dealDmg(int attackersDMG)					// return expDrop if killed, else return 0
{
	totalHp -= attackersDMG;
	if (totalHp <= 0)
	{
		if (ifHero == true)
			return -1;
		return expDrop;
	}
	return 0;
}
void CStats::setExpDrop(int _exp)
{
	expDrop = _exp;
}
void CStats::setStats(int _baseHp, int _baseDmg, int _lvl, float _hpLvlMultiplier, float _DmgLvlMultiplier, int _attackSpeed)
{
	baseHp = _baseHp;
	baseDmg = _baseDmg;
	lvl = _lvl;
	hpLvlMultiplier = _hpLvlMultiplier;
	DmgLvlMultiplier = _DmgLvlMultiplier;
	attackSpeed = _attackSpeed;
	update();
}
void CStats::setLvl(int _lvl)
{
	lvl = _lvl;
	update();
}
void CStats::addExp(int _exp)
{
	totalExp += _exp;
	if (totalExp > (100*(lvl+1)) )
	{
		lvl += 1;
		totalExp = 0;
		update();
	}
}

int CStats::getAttackSpeed()
{
	return attackSpeed;
}
CStats CStats::getStats()
{
	CStats *ret = this;
	return *ret;
}
int CStats::getTotalDmg()
{
	return totalDmg;
}
int CStats::getTotalHp()
{
	return totalHp;
}
int CStats::calculateEXP()
{
	return totalExp + ((lvl * 100 + 100) / 2)*lvl;
}
#endif