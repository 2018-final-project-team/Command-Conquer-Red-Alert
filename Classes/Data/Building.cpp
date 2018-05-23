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
Building* Building::create(Tag _tag, Vec2 _pos)
{
	//创建一个建筑物精灵对象
	Building* temp = new Building();
	//定义一个存放建筑物的字符串数组
	std::string picTable[5] = { "GameScene/Base200.png","GameScene/Power200.png","GameScene/Mine200.png","GameScene/Barracks200.png","GameScene/CarFactory200.png" };
	//初始化安装建筑物时金钱的消耗
	int sValue[5] = { 500,500,500,500,500 };


	//初始化建筑物精灵对象
	temp->initWithFile(picTable[_tag].c_str());
	//自动释放
	temp->autorelease();
	//拿到当前建筑物的_tag
	temp->_tag = _tag;
	//根据_tag设置安装各建筑物时需要的金钱
	temp->value = sValue[_tag];
	//卖掉建筑物时得到的金钱
	temp->sellValue = temp->value / 2;


	//创建一个表示选中建筑物时显示效果的精灵对象
	Sprite* scope = Sprite::create("GameScene/ring.png");
	//根据当前建筑物的大小来设置该效果精灵的尺寸
	//===========to do:根据当前建筑物的大小来设置该效果精灵的尺寸的计算式=======
	float scale = 2;
	scope->setScale(scale);
	//设置该精灵对象的位置
	scope->setPosition(_pos);
	//将该精灵对象添加到布景中
	temp->addChild(scope, 4, 1);
	//设置该精灵对象初始为不可见
	scope->setVisible(false);


	return temp;
}

