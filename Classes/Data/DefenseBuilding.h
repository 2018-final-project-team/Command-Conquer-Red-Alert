#ifndef _Defense_Building_H_
#define _Defense_Building_H_

#include "cocos2d.h"
#include "TagData.h"
#include "Data/Building.h"

class Unit;

class DefenseBuilding : public Building
{
private:
	//单位攻击力
	CC_SYNTHESIZE(int, _ATK, DefenseATK);

    //id
    CC_SYNTHESIZE(int, _id, ID);

	//单位攻击间隔
	CC_SYNTHESIZE(float, _ATKCD, DefenseATKCD);

	//攻击区域
	CC_SYNTHESIZE(unsigned int, _ATKLimit, DefenseATKLimit);
public:

	//建造建筑物的方法
	static DefenseBuilding* create(Tag tag, int id);

	/*
	*/
	void attack(Unit *);

	/*
	*/
	bool canAttack(cocos2d::Vec2);


};

#endif
