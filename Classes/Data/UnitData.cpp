//
//  UnitData.cpp
//  CCRA
//
//  Created by 王星洲 on 2018/5/23.
//

#include "UnitData.h"
#include "string"
#include "Scene/GameScene.h"
#include "cocos2d.h"
#include "math.h"
#include "Util/GameAnimation.h"

USING_NS_CC;


//创建单位,创建时按照typedef enum将对应值传入即可
Unit * Unit::create(Tag unitTag)
{
    //创建精灵
    Unit * temp = new Unit();
    
    //添加物理
    auto body = PhysicsBody::createCircle(temp -> getContentSize().width/2);
    
    body->setContactTestBitmask(1);//设置为1，开启其他刚体与此物理刚体产生碰撞，设置它才能触发碰撞检测回调函数
    
    temp -> setPhysicsBody(body);

	//=======================如果创建的是基地车===========================
	if (unitTag == BASE_CAR_TAG)
	{
		//初始化精灵对象
		temp->initWithFile("GameItem/Unit/BaseCar.png");

		//autorelease
		temp->autorelease();

		//获取对应_tag
		temp->_unitTag = unitTag;
		temp->setTag(unitTag);

		//初始化选中
		temp->_isSelected = false;

		//设置血量
		temp->_FullHP = 1000;
		temp->_HP = temp->_FullHP;

		//设置攻击力
		temp->_ATK = 0;

		//设置速度
		temp->_Speed = 15;

		//设置金钱消耗
		temp->_Value = 0;

		//设置cd时间
		temp->_CD = 0;

		//设置攻击间隔
		temp->_ATKCD = 0;

		//设置单位锁敌区域
		temp->_LockLimit = 0;

		//设置单位攻击区域
		temp->_ATKLimit = 0;

		//设置单位名称
		temp->_UnitName = "BaseCar";

		//血槽
		temp->_bloodBox = Sprite::create("GameItem/BloodBar/SoldierBloodBox.png");
		temp->_bloodBox->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
		temp->addChild(temp->_bloodBox);

		//血条
		temp->_bloodBarPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/SoldierBloodBar.png"));
		temp->_bloodBarPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
		temp->_bloodBarPt->setType(ProgressTimer::Type::BAR);
		temp->_bloodBarPt->setMidpoint(Vec2(0, 0.5));
		temp->_bloodBarPt->setPercentage(100);
		temp->addChild(temp->_bloodBarPt);

		return temp;
	}









    
    //单位图片
    std::string (picture[3]) = { "GameItem/Unit/Unit1-normal.png","GameItem/Unit/Unit2-normal.png","GameItem/Unit/Unit3-normal.png" };
    
    //初始化召唤单位是否被选中
    bool sisselected[3] = { false, false, false };
    
    //初始化召唤单位的金钱消耗
    int svalue[3] = { 50, 50, 200 };
    
    //初始化单位血量
    int shp[3] = { 100, 50, 300 };
    
    //初始化单位攻击力
    int satk[3] = { 30, 50, 100 };
    
    //初始化单位速度
    float sspeed[3] = { 0.5, 1, 1.5 };
    
    //初始化单位准备时间
    int scd[3] = { 10, 10, 30 };
    
    //初始化单位攻击间隔
    float satkcd[3] = { 0.5,0.5,1 };
    
    //初始化锁敌区域
    int slocklimit[3] = { 50, 50, 50 };
    
    //初始化攻击区域
    int satklimit[3] = { 100, 5, 100 };

    //初始化单位名字
    std::string sunitname[3] = { "infantry","dog","tank" };
    
    //初始化精灵对象
    temp -> initWithFile(picture[unitTag - 5]);
    
    //autorelease
    temp->autorelease();
    
    //获取对应_tag
    temp->_unitTag = unitTag;

    //设置精灵自身Tag
    temp->setTag(unitTag);
    
    //初始化选中
    temp->_isSelected = sisselected[unitTag - 5];

    //设置血量
    temp->_FullHP = shp[unitTag - 5];
    temp->_HP = temp->_FullHP;
    
    //设置攻击力
    temp->_ATK = satk[unitTag - 5];
    
    //设置速度
    temp->_Speed = sspeed[unitTag - 5];
    
    //设置金钱消耗
    temp->_Value = svalue[unitTag - 5];
    
    //设置cd时间
    temp->_CD = scd[unitTag - 5];
    
    //设置攻击间隔
    temp->_ATKCD = satkcd[unitTag - 5];
    
    //设置单位锁敌区域
    temp->_LockLimit = slocklimit[unitTag - 5];
    
    //设置单位攻击区域
    temp->_ATKLimit = satklimit[unitTag - 5];
    
    //设置单位名称
    temp->_UnitName = sunitname[unitTag - 5];

    //clear route
    temp->_route.clear();

    //血槽
    temp->_bloodBox = Sprite::create("GameItem/BloodBar/SoldierBloodBox.png");
    temp->_bloodBox->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
    temp->addChild(temp->_bloodBox);

    //血条
    temp->_bloodBarPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/SoldierBloodBar.png"));
    temp->_bloodBarPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
    temp->_bloodBarPt->setType(ProgressTimer::Type::BAR);
    temp->_bloodBarPt->setMidpoint(Vec2(0, 0.5));
    temp->_bloodBarPt->setPercentage(100);
    temp->addChild(temp->_bloodBarPt);
    
    return temp;
}

void Unit::moveTo(Vec2 destination)
{
    setPosition(destination);
}

void Unit::getInjuredBy(Unit * enemy)
{
    decreaseHP(enemy->_ATK);
}

void Unit::decreaseHP(int num)
{
    _HP -= num;

    auto progressTo = ProgressTo::create(0.5f, 100 * _HP / _FullHP);
    _bloodBarPt->runAction(progressTo);
}

void Unit::attack(Unit * enemy)
{
    enemy->getInjuredBy(this);
    //==========TO DO:音效=====================
}
void Unit::attack(Building *)
{

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
