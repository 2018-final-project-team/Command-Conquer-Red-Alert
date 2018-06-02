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

	_status = eIconPre;
	_isSelected = false;

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

	//_statusIcon = Sprite::create("GameItem/Panel/statusIcon.png");
	//_statusIcon->setPosition(Point(-_icon->getContentSize().width / 5, -_icon->getContentSize().height / 4));
	//_statusIcon->setVisible(false);
	//addChild(_statusIcon);
	_statusLabel = Label::createWithTTF(String::createWithFormat("Clickable")->getCString(), "fonts/Marker Felt.ttf", 22);
	_statusLabel->setColor(Color3B(255, 153, 0));
	_statusLabel->setPosition(Point(-_icon->getContentSize().width / 5, -_icon->getContentSize().height / 4));
	_statusLabel->setVisible(false);
	addChild(_statusLabel,3);

	_invalidIcon = Sprite::create("GameItem/Panel/invalid.png");
	_invalidIcon->setVisible(false);
	addChild(_invalidIcon, 1);


	setMoney(money);
	setTag(tag);

	this->retain();


}


void Icon::setStatus(IconsStatus iconSta)
{
	_status = iconSta;
	switch (iconSta)
	{
	case invalidForMoney:
		log("set not enough money");
		//_statusIcon->setVisible(false);
		_statusLabel->setString("$");
		_statusLabel->setColor(Color3B(255, 0, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(true);
		break;
	case invalidForOtherTask:
		_statusLabel->setVisible(false);
		_invalidIcon->setVisible(true);
		break;
	case eIconPre:
		//_statusIcon->setVisible(false);
		_statusLabel->setVisible(false);
		_invalidIcon->setVisible(false);
		break;
	case eIconOn:
		_statusLabel->setString("waiting");
		_statusLabel->setColor(Color3B(255, 0, 0));
		_statusLabel->setVisible(true);
		break;
	case eIconOK:
		_statusLabel->setString("OK");
		_statusLabel->setColor(Color3B(0, 255, 0));
		_statusLabel->setVisible(true);
		_invalidIcon->setVisible(false);
		break;
	}
}

void Icon::showProgressOfWait(float duration)   //单位为秒
{
	_invalidIcon->setVisible(true);

	auto progressTimer1 = ProgressTimer::create(_iconFrame);
	progressTimer1->setType(ProgressTimerType::RADIAL);//设置模式 RADIAL:半径 BAR:进度条 默认RADIAL  
	progressTimer1->setMidpoint(Vec2(0.5, 0.5));//设置百分比效果参考点 默认(0,0)  
	progressTimer1->setReverseProgress(false);//动作是否反向执行 默认false  
	progressTimer1->setReverseDirection(false);//动作是否逆序执行 默认false  
	this->addChild(progressTimer1, 2);

	auto progressTimer2 = ProgressTimer::create(_icon);
	progressTimer2->setScale(0.8);
	progressTimer2->setType(ProgressTimerType::RADIAL);//设置模式 RADIAL:半径 BAR:进度条 默认RADIAL  
	progressTimer2->setMidpoint(Vec2(0.5, 0.5));//设置百分比效果参考点 默认(0,0)  
	progressTimer2->setReverseProgress(false);//动作是否反向执行 默认false  
	progressTimer2->setReverseDirection(false);//动作是否逆序执行 默认false  
	this->addChild(progressTimer2, 2);

	auto progressTo = ProgressTo::create(duration, 100);
	progressTimer1->runAction(RepeatForever::create(progressTo->clone()));//为了方便展示 这里让动作重复执行
	progressTimer2->runAction(RepeatForever::create(progressTo->clone()));//为了方便展示 这里让动作重复执行
}