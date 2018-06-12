
/*
*  @file     Icon.cpp
*  @brief    控制图标类，控制栏中显示的图标
*  @author   王亮
*/
#include "Panel/Icon.h"
#include "cocos2d.h"
#include "Data/TagData.h"
#include "Scene/GameScene.h"

USING_NS_CC;

Icon* Icon::createIcon(Tag tag, int money, GameScene* gameScene)
{
	Icon* icon = new Icon();
	if (icon && icon->initIcon(tag, money, gameScene))
	{
		icon->autorelease();
		return icon;
	}
	CC_SAFE_DELETE(icon);
	return nullptr;

}

bool Icon::initIcon(Tag tag, int money, GameScene* gameScene)
{
	if (!Sprite::init())
	{
		return false;
	}
	_gameScene = gameScene;

	_clickToPlaceBuilding = false;

	_status = eIconPre;
	_isAble = false;

	_iconFrame = Sprite::create("GameItem/Panel/frame.png");
	addChild(_iconFrame);


	_icon = Sprite::create(String::createWithFormat("GameItem/Panel/icons_00%02d.png", tag)->getCString());
	_icon->setScale(0.8);
	addChild(_icon, 0);


	_priceIcon = Sprite::create("GameItem/Panel/priceTag.png");
	_priceIcon->setVisible(true);
	addChild(_priceIcon, 3);
	_priceLabel = Label::createWithTTF(String::createWithFormat("%d", money)->getCString(), "fonts/Marker Felt.ttf", 16);
	_priceLabel->setColor(Color3B(255, 153, 0));
	_priceLabel->setPosition(Point(_priceIcon->getContentSize().width * 0.75, _priceIcon->getContentSize().height *0.78));
	_priceIcon->addChild(_priceLabel);

	_statusLabel = Label::createWithTTF(String::createWithFormat("Clickable")->getCString(), "fonts/Marker Felt.ttf", 22);
	_statusLabel->setColor(Color3B(255, 153, 0));
	_statusLabel->setPosition(Point(-_icon->getContentSize().width / 5, -_icon->getContentSize().height / 4));
	_statusLabel->setVisible(false);
	addChild(_statusLabel, 3);

	//用于Unit的label，只有一个功能，当金钱不足是显示红色的$
	_statusLabel2 = Label::createWithTTF(String::createWithFormat("$")->getCString(), "fonts/Marker Felt.ttf", 22);
	_statusLabel2->setColor(Color3B(255, 0, 0));
	_statusLabel2->setPosition(Point(-_icon->getContentSize().width / 5, _icon->getContentSize().height / 4));
	_statusLabel2->setVisible(false);
	addChild(_statusLabel2, 3);

	_invalidIcon = Sprite::create("GameItem/Panel/invalid.png");
	_invalidIcon->setVisible(false);
	addChild(_invalidIcon, 1);


	setMoney(money);
	setIconTag(tag);

	this->retain();
}


void Icon::setStatus(IconsStatus iconSta)
{
	_status = iconSta;
	switch (iconSta)
	{
	case invalidForMoney:
		_statusLabel->setString("$");
		_statusLabel->setColor(Color3B(255, 0, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(true);
		_statusLabel2->setVisible(false);
		setIsAble(false);
		break;
	case invalidForOtherTask:
		_statusLabel->setVisible(false);
		_invalidIcon->setVisible(true);
		_statusLabel2->setVisible(false);
		setIsAble(false);
		break;
	case eIconPre:
		if (progressTimer1 != nullptr)
		{
			this->removeChild(progressTimer1, true);
		}
		if (progressTimer2)
		{
			this->removeChild(progressTimer2, true);
		}
		_statusLabel->setVisible(false);
		_invalidIcon->setVisible(false);
		_statusLabel2->setVisible(false);
		setIsAble(true);
		break;
	case eIconOn:
		_statusLabel->setString("waiting");
		_statusLabel->setColor(Color3B(255, 0, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(true);
		_statusLabel2->setVisible(false);
		setIsAble(false);
		break;
	case eIconOK:
		if (progressTimer1 != nullptr)
		{
			this->removeChild(progressTimer1, true);
		}
		if (progressTimer2)
		{
			this->removeChild(progressTimer2, true);
		}
		_statusLabel->setString("OK");
		_statusLabel->setColor(Color3B(0, 255, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(false);
		_statusLabel2->setVisible(false);
		setIsAble(false);
		break;

	case eIconPreForUnit:
		if (progressTimer1 != nullptr)
		{
			this->removeChild(progressTimer1, true);
		}
		if (progressTimer2)
		{
			this->removeChild(progressTimer2, true);
		}
		_statusLabel->setVisible(false);
		_invalidIcon->setVisible(false);
		if (getMoney() > _gameScene->getMoney())
		{
			_statusLabel2->setVisible(true);
			setIsAble(false);
		}
		else
		{
			_statusLabel2->setVisible(false);
			setIsAble(true);
		}
		break;
	case eIconOnForUnit:
		if (_iconTag == INFANTRY_TAG)
		{
			_statusLabel->setString(std::to_string(_gameScene->getInfantryNum()));
		}
		else if (_iconTag == DOG_TAG)
		{
			_statusLabel->setString(std::to_string(_gameScene->getDogNum()));
		}
		else if (_iconTag == TANK_TAG)
		{
			_statusLabel->setString(std::to_string(_gameScene->getTankNum()));
		}

		_statusLabel->setColor(Color3B(0, 255, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(true);
		if (getMoney() > _gameScene->getMoney())
		{
			_statusLabel2->setVisible(true);
			setIsAble(false);
		}
		else
		{
			_statusLabel2->setVisible(false);
			setIsAble(true);
		}
		break;
	case eIconQueuingForUnit:
		if (progressTimer1 != nullptr)
		{
			this->removeChild(progressTimer1, true);
		}
		if (progressTimer2)
		{
			this->removeChild(progressTimer2, true);
		}
		if (_iconTag == INFANTRY_TAG)
		{
			_statusLabel->setString(std::to_string(_gameScene->getInfantryNum()));
		}
		else if (_iconTag == DOG_TAG)
		{
			_statusLabel->setString(std::to_string(_gameScene->getDogNum()));
		}
		else if (_iconTag == TANK_TAG)
		{
			_statusLabel->setString(std::to_string(_gameScene->getTankNum()));
		}

		_statusLabel->setColor(Color3B(0, 255, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(true);
		if (getMoney() > _gameScene->getMoney())
		{
			_statusLabel2->setVisible(true);
			setIsAble(false);
		}
		else
		{
			_statusLabel2->setVisible(false);
			setIsAble(true);
		}
		break;
	}
}

void Icon::showProgressOfWait(float duration)   //单位为秒
{
	_invalidIcon->setVisible(true);

	if (progressTimer1 != nullptr)
	{
		this->removeChild(progressTimer1, true);
	}
	if (progressTimer2)
	{
		this->removeChild(progressTimer2, true);
	}

	progressTimer1 = ProgressTimer::create(_iconFrame);
	progressTimer1->setType(ProgressTimerType::RADIAL);//设置模式 RADIAL:半径 BAR:进度条 默认RADIAL  
	progressTimer1->setMidpoint(Vec2(0.5, 0.5));//设置百分比效果参考点 默认(0,0)  
	progressTimer1->setReverseProgress(false);//动作是否反向执行 默认false  
	progressTimer1->setReverseDirection(false);//动作是否逆序执行 默认false  

	progressTimer2 = ProgressTimer::create(_icon);
	progressTimer2->setScale(0.8);
	progressTimer2->setType(ProgressTimerType::RADIAL);//设置模式 RADIAL:半径 BAR:进度条 默认RADIAL  
	progressTimer2->setMidpoint(Vec2(0.5, 0.5));//设置百分比效果参考点 默认(0,0)  
	progressTimer2->setReverseProgress(false);//动作是否反向执行 默认false  
	progressTimer2->setReverseDirection(false);//动作是否逆序执行 默认false  

	this->addChild(progressTimer1, 2);
	this->addChild(progressTimer2, 2);

	auto progressTo = ProgressTo::create(duration, 100);
	progressTimer1->runAction(RepeatForever::create(progressTo->clone()));
	progressTimer2->runAction(RepeatForever::create(progressTo->clone()));


}


IconsStatus Icon::getStatus()
{
	return _status;
}

