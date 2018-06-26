//
//  UnitData.cpp
//

#include "UnitData.h"
#include "string"
#include "Scene/GameScene.h"
#include "cocos2d.h"
#include "math.h"
#include "Util/GameAnimation.h"
#include "Util/GameAudio.h"


USING_NS_CC;


//创建单位,创建时按照typedef enum将对应值传入即可
Unit * Unit::create(Tag unitTag, int id, int index)
{
    //创建精灵
    Unit * temp = new Unit();
    
    //添加物理
    auto body = PhysicsBody::createCircle(temp -> getContentSize().width/2);
    
    body->setContactTestBitmask(1);//设置为1，开启其他刚体与此物理刚体产生碰撞，设置它才能触发碰撞检测回调函数
    
    temp -> setPhysicsBody(body);

	temp->setLastTurn(stateWalkRight);

    temp->setID(id);

    temp->setIndex(index);

	//=======================如果创建的是基地车===========================
	if (unitTag == BASE_CAR_TAG)
	{
		//初始化精灵对象
		temp->initWithFile("GameItem/Unit/BaseCar-normal.png");

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
		temp->_Speed = 0.15;

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

		//红色血条
		temp->_bloodBarAsEnemyPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/SoldierBloodBarEnemy.png"));
		temp->_bloodBarAsEnemyPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
		temp->_bloodBarAsEnemyPt->setType(ProgressTimer::Type::BAR);
		temp->_bloodBarAsEnemyPt->setMidpoint(Vec2(0, 0.5));
		temp->_bloodBarAsEnemyPt->setPercentage(100);
		temp->_bloodBarAsEnemyPt->setVisible(false);       //默认不显示
		temp->addChild(temp->_bloodBarAsEnemyPt);

		temp->_ring = Sprite::create("GameItem/ring/ring.png");
		temp->_ring->setScale(temp->getContentSize().width / 30);
		temp->_ring->setPosition(Vec2(temp->getContentSize().width / 2, 3));
		temp->_ring->setVisible(false);
		temp->addChild(temp->_ring, -1);

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
    float sspeed[3] = { 0.08, 0.12, 0.15 };
    
    //初始化单位准备时间
    int scd[3] = { 10, 10, 30 };
    
    //初始化单位攻击间隔
    float satkcd[3] = { 1000, 1000, 2000 };
    
    //初始化锁敌区域
    int slocklimit[3] = { 50, 50, 50 };
    
    //初始化攻击区域
    int satklimit[3] = { 120, 45, 180 };

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

	//红色血条
	temp->_bloodBarAsEnemyPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/SoldierBloodBarEnemy.png"));
	temp->_bloodBarAsEnemyPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->_bloodBarAsEnemyPt->setType(ProgressTimer::Type::BAR);
	temp->_bloodBarAsEnemyPt->setMidpoint(Vec2(0, 0.5));
	temp->_bloodBarAsEnemyPt->setPercentage(100);
	temp->_bloodBarAsEnemyPt->setVisible(false);       //默认不显示
	temp->addChild(temp->_bloodBarAsEnemyPt);

	temp->_ring = Sprite::create("GameItem/ring/ring.png");
	temp->_ring->setScale(temp->getContentSize().width / 30);
	temp->_ring->setPosition(Vec2(temp->getContentSize().width / 2, 1));
	temp->_ring->setVisible(false);
	temp->addChild(temp->_ring, -1);

	//temp->runAction(RepeatForever::create(Animate::create((AnimationCache::getInstance()->getAnimation("infantry_right")))));
    
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

void Unit::getInjuredBy(DefenseBuilding * enemy)
{
	decreaseHP(enemy->getDefenseATK());
}

void Unit::decreaseHP(int num)
{
    _HP -= num;

    auto progressTo = ProgressTo::create(0.5f, 100 * _HP / _FullHP);
	if (_bloodBarPt->isVisible())
	{
		_bloodBarPt->runAction(progressTo);
	}
	else
	{
		_bloodBarAsEnemyPt->runAction(progressTo);
	}
}

void Unit::attack(Unit * enemy)
{
    enemy->getInjuredBy(this);
    //==========TO DO:音效=====================
	GameAudio::getInstance()->playEffect("Sound/Explosion.mp3");
}

void Unit::attack(Building * enemy)
{
	enemy->getInjuredBy(this);
    //==========TO DO:音效=====================
    GameAudio::getInstance()->playEffect("Sound/Explosion.mp3");
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


void Unit::switchState(UnitState state)
{
	setUnitState(state);
	switch (state)
	{
	case stateNone:
		changeToDefault();
		break;
	case stateWalkRight:
		setLastTurn(state);
		changeToRight();
		break;
	case stateWalkLeft:
		setLastTurn(state);
		changeToLeft();
		break;
	case stateWalkUp:
		setLastTurn(state);
		changeToUp();
		break;
	case stateWalkDown:
		setLastTurn(state);
		changeToDown();
		break;
	case stateAttackLeft:
		setLastTurn(state);
		changeToAttackLeft();
		break;
	case stateAttackRight:
		setLastTurn(state);
		changeToAttackRight();
		break;
	case stateDeath:
		changeToDead();
		break;

	}
}


//改变状态为默认状态
void Unit::changeToDefault()
{
	switch (getLastTurn())
	{
	case stateAttackRight:
	case stateWalkRight:
		switch (getUnitTag())
		{
		case INFANTRY_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit1-attackRight (1).png"));
			break;
		case DOG_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit2-attackRight (2).png"));
			break;
		case TANK_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit3-right (3).png"));
			break;
		case BASE_CAR_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BaseCar-right (1).png"));
			break;
		}
		break;
	case stateAttackLeft:
	case stateWalkLeft:
		switch (getUnitTag())
		{
		case INFANTRY_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit1-attackLeft (1).png"));
			break;
		case DOG_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit2-attackLeft (2).png"));
			break;
		case TANK_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit3-left (3).png"));
			break;
		case BASE_CAR_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BaseCar-left (1).png"));
			break;
		}
		break;
	case stateWalkUp:
		switch (getUnitTag())
		{
		case INFANTRY_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit1-back (1).png"));
			break;
		case DOG_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit2-back (1).png"));
			break;
		case TANK_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit3-back (1).png"));
			break;
		case BASE_CAR_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BaseCar-back (1).png"));
			break;
		}
		break;
	case stateWalkDown:
		switch (getUnitTag())
		{
		case INFANTRY_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit1-forward (1).png"));
			break;
		case DOG_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit2-forward (1).png"));
			break;
		case TANK_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Unit3-forward (1).png"));
			break;
		case BASE_CAR_TAG:
			stopAllActions();
			setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BaseCar-forward (1).png"));
			break;
		}
		break;
	
	default:
		break;
	}
}

//改变状态为攻击状态
void Unit::changeToAttackLeft()
{
	if (getUnitTag() == BASE_CAR_TAG)   //基地车没有攻击状态
	{
		stopAllActions();
		setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BaseCar-left (1).png"));
	}
	else
	{
		stopAllActions();
		runAction(Animate::create(
			(AnimationCache::getInstance()->getAnimation(getUnitName() + "_attackLeft"))));
	}
}

void Unit::changeToAttackRight()
{
	if (getUnitTag() == BASE_CAR_TAG)   //基地车没有攻击状态
	{
		stopAllActions();
		setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("BaseCar-right (1).png"));
	}
	else
	{
		stopAllActions();
		runAction(Animate::create(
			(AnimationCache::getInstance()->getAnimation(getUnitName() + "_attackRight"))));
	}
}

//改变朝向
void Unit::changeToUp()
{
	stopAllActions();
	runAction(RepeatForever::create(Animate::create(
		(AnimationCache::getInstance()->getAnimation(getUnitName() + "_back")))));
}

void Unit::changeToLeft()
{
	stopAllActions();
	runAction(RepeatForever::create(Animate::create(
		(AnimationCache::getInstance()->getAnimation(getUnitName() + "_left")))));
}

void Unit::changeToDown()
{
	stopAllActions();
	runAction(RepeatForever::create(Animate::create(
		(AnimationCache::getInstance()->getAnimation(getUnitName() + "_forward")))));
}

void Unit::changeToRight()
{
	log("turn right");
	stopAllActions();
	runAction(RepeatForever::create(Animate::create(
		(AnimationCache::getInstance()->getAnimation(getUnitName() + "_right")))));
}


//改变状态为死亡(播放死亡过程的动画)
//TODO:士兵死亡的更多处理
void Unit::changeToDead()
{
	if (getUnitTag() != BASE_CAR_TAG)   //基地车暂时没有死亡动画
	{
		stopAllActions();
		runAction(Animate::create(
			(AnimationCache::getInstance()->getAnimation(getUnitName() + "_die"))));
	}
}

bool Unit::getIsSelected()
{
	return _isSelected;
}


void Unit::setIsSelected(bool b)
{
	_isSelected = b;
	if (b)
	{
		_ring->setVisible(true);
	}
	else
	{
		_ring->setVisible(false);
	}
}
