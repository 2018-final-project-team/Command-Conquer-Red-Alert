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

namespace buildingData
{
    const int powerPlantCastMoney = 200; 
    const int powerPlantCastPower = -1000;
    const int mineCastMoney = 1000;
    const int mineCastPower = 600;
    const int barracksCastMoney = 500;
    const int barracksCastPower = 300;
    const int carFactoryCastMoney = 800;
    const int carFactoryCastPower = 500;

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

	//建筑物的tag（基地、电厂、矿场、兵营、战车工厂）
	CC_SYNTHESIZE(Tag, _buildingTag, BuildingTag);

    //耗费的电
    CC_SYNTHESIZE(int, _castPower, CastPower);

	////建筑的级别，1，2，3
	//int level;

	//建筑物的血量
	CC_SYNTHESIZE(int, _hp, HP);
	//建造建筑物需要的金钱数
	CC_SYNTHESIZE(int, _value, BuildingValue);
	////升级建筑物需要的金钱数
	//int upValue;
	//卖掉建筑物得到的金钱
	CC_SYNTHESIZE(int, _sellValue, SellValue);

	////更新建筑物的标志位
	//bool updateMark;

	//建造建筑物的方法
	static Building* create(Tag _tag);


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
