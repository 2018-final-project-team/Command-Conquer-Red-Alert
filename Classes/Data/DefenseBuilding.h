#ifndef _Defense_Building_H_
#define _Defense_Building_H_

#include "cocos2d.h"
#include "TagData.h"
#include "Data/Building.h"

class Unit;

class DefenseBuilding : public Building
{
private:
	//��λ������
	CC_SYNTHESIZE(int, _ATK, DefenseATK);

	//��λ�������
	CC_SYNTHESIZE(float, _ATKCD, DefenseATKCD);

	//��������
	CC_SYNTHESIZE(unsigned int, _ATKLimit, DefenseATKLimit);
public:

	//���콨����ķ���
	static DefenseBuilding* create(Tag tag);

	/*
	*/
	void attack(Unit *);

	/*
	*/
	bool canAttack(cocos2d::Vec2);


};

#endif
