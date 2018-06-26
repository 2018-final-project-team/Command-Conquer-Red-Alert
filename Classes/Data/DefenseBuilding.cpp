#include "DefenseBuilding.h"
#include "Scene/GameScene.h"
#include "cocos2d.h"
#include "Util/GameAudio.h"


using namespace cocos2d;


//创建建筑物时调用的方法
DefenseBuilding* DefenseBuilding::create(Tag tag, int id)
{
	//创建一个建筑物精灵对象
	DefenseBuilding* temp = new DefenseBuilding();
	//添加物理
	auto body = PhysicsBody::createCircle(temp->getContentSize().width / 2);

	body->setContactTestBitmask(1);//设置为1，开启其他刚体与此物理刚体产生碰撞，设置它才能触发碰撞检测回调函数

	temp->setPhysicsBody(body);

    temp->setID(id);

	//初始化建筑物精灵对象
	temp->initWithFile("GameItem/Building/Defense200.png");
	//自动释放
	temp->autorelease();
	//拿到当前建筑物的_tag
	temp->_buildingTag = tag;
	//设置精灵自身Tag
	temp->setTag(tag);

	//设置血量
	temp->_FullHp = 300;
	temp->_hp = temp->_FullHp;
	//根据_tag设置安装各建筑物时需要的金钱、电量
	temp->_value = 200;
	temp->_castPower = 100;
	//卖掉建筑物时得到的金钱
	temp->_sellValue = temp->_value / 2;

	//设置攻击力
	temp->_ATK = 300;

	//设置攻击间隔
	temp->_ATKCD = 2000;

	//设置单位攻击区域
	temp->_ATKLimit = 350;

	//血槽
	temp->_bloodBox = Sprite::create("GameItem/BloodBar/BuildingBloodBox.png");
	temp->_bloodBox->setScale(temp->getContentSize().width / 200);
	temp->_bloodBox->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->addChild(temp->_bloodBox);

	//血条
	temp->_bloodBarPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/BuildingBloodBar.png"));
	temp->_bloodBarPt->setScale(temp->getContentSize().width / 200);
	temp->_bloodBarPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->_bloodBarPt->setType(ProgressTimer::Type::BAR);
	temp->_bloodBarPt->setMidpoint(Vec2(0, 0.5));
	temp->_bloodBarPt->setPercentage(100);
	temp->addChild(temp->_bloodBarPt);

	//红色血条
	temp->_bloodBarAsEnemyPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/BuildingBloodBarEnemy.png"));
	temp->_bloodBarAsEnemyPt->setScale(temp->getContentSize().width / 200);
	temp->_bloodBarAsEnemyPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->_bloodBarAsEnemyPt->setType(ProgressTimer::Type::BAR);
	temp->_bloodBarAsEnemyPt->setMidpoint(Vec2(0, 0.5));
	temp->_bloodBarAsEnemyPt->setPercentage(100);
	temp->_bloodBarAsEnemyPt->setVisible(false);    //默认不显示
	temp->addChild(temp->_bloodBarAsEnemyPt);

	return temp;
}


void DefenseBuilding::attack(Unit * enemy)
{
	enemy->getInjuredBy(this);
	//==========TO DO:音效=====================
	GameAudio::getInstance()->playEffect("Sound/Explosion.mp3");
}


bool DefenseBuilding::canAttack(Vec2 position)
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