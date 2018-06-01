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
	isSelected = false;

	_iconFrame = Sprite::create("GameItem/Panel/frame.png");
	addChild(_iconFrame);


	auto test = String::createWithFormat("GameItem/Panel/icons_00%02d.png", tag)->getCString();
	_icon = Sprite::create(String::createWithFormat("GameItem/Panel/icons_00%02d.png", tag)->getCString());
	log("succesfully create icon:%s", test);
	_icon->setScale(0.8);
	addChild(_icon);
	_priceIcon = Sprite::create("GameItem/Panel/priceIcon.png");
	_priceIcon->setPosition(Point(_icon->getContentSize().width / 5, -_icon->getContentSize().height / 4));
	addChild(_priceIcon);
	_priceLabel = Label::createWithTTF(String::createWithFormat("%d", money)->getCString(), "fonts/arial.ttf", 20);
	_priceLabel->setColor(Color3B(255, 153, 0));
	_priceLabel->setPosition(Point(_priceIcon->getContentSize().width / 2, _priceIcon->getContentSize().height / 2 - 2));
	_priceIcon->addChild(_priceLabel);


	////=============TO DO:添加OK.PNG============================
	//_okIcon = Sprite::createWithSpriteFrameName(".png");
	//_okIcon->setPosition(Point(_icon->getContentSize().width / 2, _icon->getContentSize().height / 2));
	//_icon->addChild(_okIcon);
	//_okIcon->setVisible(false);

	setMoney(money);
	setTag(tag);
	/*this->scheduleUpdate();*/
}


void Icon::update(float dt)
{
	if (/*!(_gameScene->_manager->_isWaitToCreateBuilding) &&*/ getMoney() <= _gameScene->getMoney())
	{
		setNotSelected();
	}
	else 
	{
		setNotEnoughMoney();
	}
}

void Icon::setNotSelected()
{

}

void Icon::setNotEnoughMoney()
{

}