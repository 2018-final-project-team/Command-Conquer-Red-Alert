//
//  UnitData.cpp
//  CCRA
//
//  Created by 王星洲 on 2018/5/23.
//

#include "UnitData.h"
#include <string>
#include "Scene/GameScene.h"
#include "cocos2d.h"
#include <math.h>

USING_NS_CC;


//创建单位,创建时按照typedef enum将对应值传入即可
Unit * Unit::create(Tag unitTag)
{
    //创建精灵
    Unit * temp = new Unit();

    //单位图片
    std::string picture[3] = {};

    //初始化召唤单位是否被选中
    bool sisselected[3] = { false, false, false };

    //初始化召唤单位的金钱消耗
    int svalue[3] = { 50, 50, 200 };

    //初始化单位血量
    int shp[3] = { 100, 50, 300 };

    //初始化单位攻击力
    int satk[3] = { 30, 50, 100 };

    //初始化单位速度
    int sspeed[3] = { 5, 10, 15 };

    //初始化单位准备时间
    int scd[3] = { 10, 10, 30 };

    //初始化单位攻击间隔
    float satkcd[3] = { 0.5,0.5,1 };

    //初始化锁敌区域
    int slocklimit[3] = { 50, 50, 50 };

    //初始化攻击区域
    int satklimit[3] = { 20, 5, 30 };

    //初始化单位名字
    std::string sunitname[3] = { "infantry","dog","tank" };

    //初始化精灵对象
    temp->initWithFile(picture[unitTag].c_str());

    //autorelease
    temp->autorelease();

    //获取对应_tag
    temp->_unitTag = unitTag;

    //初始化选中
    temp->_isSelected = sisselected[unitTag];

    //设置血量
    temp->_HP = shp[unitTag];

    //设置攻击力
    temp->_ATK = satk[unitTag];

    //设置速度
    temp->_Speed = sspeed[unitTag];

    //设置金钱消耗
    temp->_Value = svalue[unitTag];

    //设置cd时间
    temp->_CD = scd[unitTag];

    //设置攻击间隔
    temp->_ATKCD = satkcd[unitTag];

    //设置单位锁敌区域
    temp->_LockLimit = slocklimit[unitTag];

    //设置单位攻击区域
    temp->_ATKLimit = satklimit[unitTag];

    //设置单位名称
    temp->_UnitName = sunitname[unitTag];

    return temp;
}

void Unit::moveTo(Vec2 destination, float time)
{
    auto move = MoveTo::create(time, destination);
    this->runAction(move);
}

void Unit::getInjuredBy(Unit * enemy)
{
    _HP -= enemy->_ATK;
}

void Unit::attack(Unit * enemy)
{
    enemy->getInjuredBy(this);
}


bool Unit::canAttack(Vec2 position)
{
    Vec2 myPosition = this->getPosition();
    float distance = sqrt(pow(myPosition.x - position.x, 2) + pow(myPosition.y - position.y, 2));
    if (distance <= _ATKLimit)
    {
        return true;
    }
    else
    {
        return false;
    }
}
