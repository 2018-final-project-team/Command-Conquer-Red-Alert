//
//  Building.h
//  CCRA
//
//  Created by 王星洲 on 2018/5/23.
//  Modified by 王亮 on 2018/5/23
//

#ifndef _Building_H_
#define _Building_H_

#include "cocos2d.h"
#include "TagData.h"

class Unit;

namespace buildingData
{
    const int powerPlantCostMoney = 500; 
    const int powerPlantCostPower = 0;
    const int mineCostMoney = 500;
    const int mineCostPower = 600;
    const int barracksCostMoney = 500;
    const int barracksCostPower = 300;
    const int carFactoryCostMoney = 500;
    const int carFactoryCostPower = 500;

    namespace EnoughPower
    {
        const int powerPlantWait = 5*1000;
        const int mineWait = 5 * 1000;
        const int barracksWait = 5 * 1000;
        const int carFactoryWait = 5 * 1000;
    }

    namespace NotEnoughPower
    {
        const int powerPlantWait = 10*1000;
        const int mineWait = 10 * 1000;
        const int barracksWait = 10 * 1000;
        const int carFactoryWait = 10 * 1000;
    }

}

class Building : public cocos2d::Sprite
{
public:
    cocos2d::Sprite* _bloodBox;           //血槽Sprite
    cocos2d::ProgressTimer* _bloodBarPt;     //血条ProgressTimer

	//建筑物的tag（基地、电厂、矿场、兵营、战车工厂）
	CC_SYNTHESIZE(Tag, _buildingTag, BuildingTag);

    //耗费的电
    CC_SYNTHESIZE(int, _castPower, CastPower);

	////建筑的级别，1，2，3
	//int level;

	//建筑物的当前血量
	CC_SYNTHESIZE(int, _hp, HP);
	//建筑物的满血血量
	CC_SYNTHESIZE(int, _FullHp, FullHP);
	//建造建筑物需要的金钱数
	CC_SYNTHESIZE(int, _value, BuildingValue);
	////升级建筑物需要的金钱数
	//int upValue;
	//卖掉建筑物得到的金钱
	CC_SYNTHESIZE(int, _sellValue, SellValue);

	////更新建筑物的标志位
	//bool updateMark;

	//建造建筑物的方法
	static Building* create(Tag tag);


	void getInjuredBy(Unit *);

	void decreaseHP(int num);

	//////升级建筑物的方法
	////void updateData();
	////升级建筑物前的准备方法
	//void update();


	//系统定义的一个宏，做好相应的初始化与释放工作
    //CREATE_FUNC(Building);

public:
    //To Do: 销毁函数 等待修改
    void setDeath() { ; }


};

#endif
