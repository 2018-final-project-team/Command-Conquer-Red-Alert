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
#include "Scene/GameScene.h"

class Building : public cocos2d::Sprite
{

public:
	//构造函数
	Building();

	//建筑物的id， 1，2，3，4,5（基地、电厂、矿场、兵营、战车工厂）
	Tag _tag;

	////建筑的级别，1，2，3
	//int level;

	//建造建筑物需要的金钱数
	int value;
	////升级建筑物需要的金钱数
	//int upValue;
	//卖掉建筑物得到的金钱
	int sellValue;

	////更新建筑物的标志位
	//bool updateMark;

	//建造建筑物的方法
	static Building* create(Tag _tag, cocos2d::Vec2 _pos);


	//////升级建筑物的方法
	////void updateData();
	////升级建筑物前的准备方法
	//void update();


	//系统定义的一个宏，做好相应的初始化与释放工作
	CREATE_FUNC(Building);


};

#endif