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

USING_NS_CC;

//构造函数
Unit::Unit(){}


//创建单位,创建时按照typedef enum将对应值传入即可
Unit * Unit::create(Tag _tag)
{
    //创建精灵
    Unit * temp = new Unit();
    
    //单位图片
    std::string picture[3] = {};
    
    //初始化召唤单位的金钱消耗
    int sValue[3] = {50, 50, 200};
    
    //初始化单位血量
    int sHP[3] = {100, 50, 300};
    
    //初始化单位攻击力
    int sATK[3] = {30, 50, 100};
    
    //初始化单位速度
    int sSpeed[3] = {5, 10, 15};
    
    //初始化单位准备时间
    int sCD[3] = {10, 10, 30};
    
    //初始化锁敌区域
    int sLockLimit[3] = {50, 50, 50};
    
    //初始化攻击区域
    int sATKLimit[3] = {20, 20, 30};
    
    //初始化单位名字
    std::string sUnitName[3] = {"步兵","狗","坦克"};
    
    //初始化精灵对象
    temp -> initWithFile(picture[_tag].c_str());
   
    //获取对应_tag
    temp -> _tag = _tag - 5;
    
    //设置血量
    temp -> _HP = sHP[_tag];
    
    //设置攻击力
    temp -> _ATK = sATK[_tag];
    
    //设置速度
    temp -> _Speed = sSpeed[_tag];
    
    //设置金钱消耗
    temp -> _Value = sValue[_tag];
    
    //设置CD时间
    temp -> _CD = sCD[_tag];
    
    //设置单位锁敌区域
    temp -> _LockLimit = sLockLimit[_tag];
    
    //设置单位攻击区域
    temp -> _ATKLimit = sATKLimit[_tag];
    
    //设置单位名称
    temp -> _UnitName = sUnitName[_tag];
    
    return temp;
}

bool Unit::init()
{
    
    
    return true;
}



