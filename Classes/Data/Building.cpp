//
//  BuildingData.cpp
//  CCRA
//
//  Created by 王星洲 on 2018/5/23.
//  Modified by 王亮 on 2018/5/23
//

#include "Building.h"
#include "Scene/GameScene.h"
#include "cocos2d.h"


using namespace cocos2d;

//构造函数
Building::Building() {}


//创建建筑物时调用的方法
Building* Building::create(Tag _tag)
{
	//创建一个建筑物精灵对象
	Building* temp = new Building();
	//定义一个存放建筑物的字符串数组
	std::string picTable[5] = { "GameScene/Base200.png","GameScene/Power200.png","GameScene/Mine200.png","GameScene/Barracks200.png","GameScene/CarFactory200.png" };
	//初始化安装建筑物时金钱的消耗
	int sValue[5] = { 500,500,500,500,500 };
	//初始化建筑物的血量
	int sHP[5] = { 2000,500,500,500,500 };


	//初始化建筑物精灵对象
	temp->initWithFile(picTable[_tag].c_str());
	//自动释放
	temp->autorelease();
	//拿到当前建筑物的_tag
	temp->_tag = _tag;
	//设置血量
	temp->_hp = sHP[_tag];
	//根据_tag设置安装各建筑物时需要的金钱
	temp->_value = sValue[_tag];
	//卖掉建筑物时得到的金钱
	temp->_sellValue = temp->_value / 2;

	return temp;
}

