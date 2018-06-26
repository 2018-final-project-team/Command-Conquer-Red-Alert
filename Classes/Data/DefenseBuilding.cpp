#include "DefenseBuilding.h"
#include "Scene/GameScene.h"
#include "cocos2d.h"
#include "Util/GameAudio.h"


using namespace cocos2d;


//����������ʱ���õķ���
DefenseBuilding* DefenseBuilding::create(Tag tag, int id)
{
	//����һ�������ﾫ�����
	DefenseBuilding* temp = new DefenseBuilding();
	//�������
	auto body = PhysicsBody::createCircle(temp->getContentSize().width / 2);

	body->setContactTestBitmask(1);//����Ϊ1�����������������������������ײ�����������ܴ�����ײ���ص�����

	temp->setPhysicsBody(body);

    temp->setID(id);

	//��ʼ�������ﾫ�����
	temp->initWithFile("GameItem/Building/Defense200.png");
	//�Զ��ͷ�
	temp->autorelease();
	//�õ���ǰ�������_tag
	temp->_buildingTag = tag;
	//���þ�������Tag
	temp->setTag(tag);

	//����Ѫ��
	temp->_FullHp = 300;
	temp->_hp = temp->_FullHp;
	//����_tag���ð�װ��������ʱ��Ҫ�Ľ�Ǯ������
	temp->_value = 200;
	temp->_castPower = 100;
	//����������ʱ�õ��Ľ�Ǯ
	temp->_sellValue = temp->_value / 2;

	//���ù�����
	temp->_ATK = 300;

	//���ù������
	temp->_ATKCD = 2000;

	//���õ�λ��������
	temp->_ATKLimit = 350;

	//Ѫ��
	temp->_bloodBox = Sprite::create("GameItem/BloodBar/BuildingBloodBox.png");
	temp->_bloodBox->setScale(temp->getContentSize().width / 200);
	temp->_bloodBox->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->addChild(temp->_bloodBox);

	//Ѫ��
	temp->_bloodBarPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/BuildingBloodBar.png"));
	temp->_bloodBarPt->setScale(temp->getContentSize().width / 200);
	temp->_bloodBarPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->_bloodBarPt->setType(ProgressTimer::Type::BAR);
	temp->_bloodBarPt->setMidpoint(Vec2(0, 0.5));
	temp->_bloodBarPt->setPercentage(100);
	temp->addChild(temp->_bloodBarPt);

	//��ɫѪ��
	temp->_bloodBarAsEnemyPt = ProgressTimer::create(Sprite::create("GameItem/BloodBar/BuildingBloodBarEnemy.png"));
	temp->_bloodBarAsEnemyPt->setScale(temp->getContentSize().width / 200);
	temp->_bloodBarAsEnemyPt->setPosition(Vec2(temp->getContentSize().width / 2, temp->getContentSize().height + 10));
	temp->_bloodBarAsEnemyPt->setType(ProgressTimer::Type::BAR);
	temp->_bloodBarAsEnemyPt->setMidpoint(Vec2(0, 0.5));
	temp->_bloodBarAsEnemyPt->setPercentage(100);
	temp->_bloodBarAsEnemyPt->setVisible(false);    //Ĭ�ϲ���ʾ
	temp->addChild(temp->_bloodBarAsEnemyPt);

	return temp;
}


void DefenseBuilding::attack(Unit * enemy)
{
	enemy->getInjuredBy(this);
	//==========TO DO:��Ч=====================
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