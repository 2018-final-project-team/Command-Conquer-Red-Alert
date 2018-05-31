/*
*  @file     Panel.cpp
*  @brief    控制栏类，使用控制栏进行建造建筑、训练士兵、制造战车
*  @author   王亮
*/

#include "cocos2d.h"
#include "Panel/Panel.h"
#include "Panel/Icon.h"
#include "Scene/GameScene.h"

//#include "Manager/GameManager.h"

USING_NS_CC;

Panel* Panel::createWithGameScene(GameScene* gameScene)
{
	
	Panel* panel = new Panel();
	if (panel->initWithGameScene(gameScene))
	{
		panel->autorelease();
		return panel;
	}

	CC_SAFE_DELETE(panel);
	return nullptr;

}

bool Panel::initWithGameScene(GameScene* gameScene)
{
	if (!Sprite::init())
	{
		return false;
	}

	_gameScene = gameScene;
	_curCategoryTag = BUILDING_BUTTON;
	//TO DO:添加工具栏背景图片
	_panelBG = Sprite::create("GameItem/Panel/panelBG_normal.png");
	_panelBG->setPosition(Point(0, 0));
	addChild(_panelBG);

	_curCategoryTag = BUILDING_BUTTON;

	//TO DO:添加工具栏三个分类按钮的图片
	_buildingButton = Sprite::create("GameItem/Panel/B.png");
	_buildingButton->setPosition(Point(-60,0));
	_buildingButton->setTag(BUILDING_BUTTON);
	addChild(_buildingButton);

	_soldierButton = Sprite::create("GameItem/Panel/S.png");
	_soldierButton->setPosition(Point(0,0));
	_soldierButton->setTag(SOLDIER_BUTTON);
	addChild(_soldierButton);

	_carButton = Sprite::create("GameItem/Panel/C.png");
	_carButton->setPosition(Point(60,0));
	_carButton->setTag(CAR_BUTTON);
	addChild(_carButton);

	addIcons();


	//===============注册监听器===================
	_mainButtonListener = EventListenerTouchOneByOne::create();
    _mainButtonListener->onTouchBegan = [this](Touch* touch, Event* event) {
		Sprite* _selectedButton = static_cast<Sprite*>(event->getCurrentTarget());
		Vec2 locationInNode = _selectedButton->convertToNodeSpace(touch->getLocation());
		Size s = _selectedButton->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		
		//点击范围判断检测
		if (rect.containsPoint(locationInNode))
		{
			switch (_selectedButton->getTag())
			{
			case BUILDING_BUTTON:
				log("select building button, the tag is:%d", _selectedButton->getTag());
				setCurButton(BUILDING_BUTTON);
				break;
			case SOLDIER_BUTTON:
				log("select soldier button");
				setCurButton(SOLDIER_BUTTON);
				break;
			case CAR_BUTTON:
				log("select car button");
				setCurButton(CAR_BUTTON);
				break;
			}

			return true;
		}
		return false;
    };
	_mainButtonListener->setSwallowTouches(true);   //吞没触摸事件，不向下传递
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener, _buildingButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener->clone(), _soldierButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener->clone(), _carButton);

	//=================TO DO:监听器的注销===============================

	_buildingList = Vector<Icon*>();
	_soldierList = Vector<Icon*>();
	_carList = Vector<Icon*>();

	

	return true;
}


//TO DO:面板的调整
void Panel::setCurButton(Tag tag)
{
	_curCategoryTag = tag;

	checkIcon(tag);
	showIcon(tag);
	log("the size of _curList is: %d", _curList->size());

}

void Panel::checkIcon(Tag tag)
{
	removeAllIcon();     //不再显示原来的图标

	//=======_curList指针指向新的要显示的列表，并删除原有元素===========
	switch (tag)
	{
	case BUILDING_BUTTON:
		_curList = &_buildingList;
		break;
	case SOLDIER_BUTTON:
		_curList = &_soldierList;
		break;
	case CAR_BUTTON:
		_curList = &_carList;
		break;
	}
	_curList->clear();

	//=========测试代码：虚拟各种图标出现的条件========
	_gameScene->setIsBaseExist(true);
	_gameScene->setPowerPlantNum(1);
	_gameScene->setBarracksNum(1);
	_gameScene->setCarFactoryNum(1);
	
	switch (tag)
	{
	case BUILDING_BUTTON:
		if (_gameScene->getIsBaseExist())
		{
			_buildingList.pushBack(_powerPlantIcon);
			_buildingList.pushBack(_mineIcon);
		}

		if (_gameScene->getPowerPlantNum())
		{
			_buildingList.pushBack(_barracksIcon);
			_buildingList.pushBack(_carFactoryIcon);
		}
		break;
	case SOLDIER_BUTTON:
		_soldierList.empty();
		if (_gameScene->getBarracksNum())
		{
			_soldierList.pushBack(_infantryIcon);
			_soldierList.pushBack(_dogIcon);
		}
		break;
	case CAR_BUTTON:
		_carList.empty();
		if (_gameScene->getCarFactoryNum())
		{
			_carList.pushBack(_tankIcon);
		}
		break;
	}
}

void Panel::showIcon(Tag tag)
{
	ssize_t _needToShow = _curList->size();
	

	//================TO DO:坐标数据合理==================
	for (int i=0;i<_needToShow;i++)
	{
		float x = 0;
		float y = 0 - (i+1) * 80;
		_curList->at(i)->setPosition(Vec2(x, y));
		this->addChild(_curList->at(i));
	}
}


void Panel::removeAllIcon()
{
	if (_curList != NULL)
	{
		for (int i = 0; i<_curList->size(); i++)
		{
			this->removeChild(_curList->at(i), false);    //TO DO:确定true or false
		}
	}
}

void Panel::addIcons()
{
	_powerPlantIcon = Icon::createIcon(POWER_PLANT_TAG, sValue[POWER_PLANT_TAG -1], _gameScene);
	_mineIcon = Icon::createIcon(MINE_TAG, sValue[MINE_TAG - 1], _gameScene);
	_barracksIcon = Icon::createIcon(BARRACKS_TAG, sValue[BARRACKS_TAG - 1], _gameScene);
	_carFactoryIcon = Icon::createIcon(CAR_FACTORY_TAG, sValue[CAR_FACTORY_TAG - 1], _gameScene);
	_infantryIcon = Icon::createIcon(INFANTRY_TAG, sValue[INFANTRY_TAG - 1], _gameScene);
	_dogIcon = Icon::createIcon(DOG_TAG, sValue[DOG_TAG - 1], _gameScene);
	_tankIcon = Icon::createIcon(TANK_TAG, sValue[TANK_TAG - 1], _gameScene);
}