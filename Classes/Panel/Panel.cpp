/*
*  @file     Panel.cpp
*  @brief    控制栏类，使用控制栏进行建造建筑、训练士兵、制造战车
*  @author   王亮
*/

#include "cocos2d.h"
#include "Panel/Panel.h"
#include "Panel/Icon.h"
#include "Scene/GameScene.h"
#include <time.h>

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


	_clickToPlaceBuilding = false;

	_gameScene = gameScene;
	_curCategoryTag = BUILDING_BUTTON;

	//===================添加工具栏背景图片===========================
	_panelBG = Sprite::create("GameItem/Panel/panelBG_normal.png");
	_panelBG->setPosition(Point(0, 0));
	addChild(_panelBG);

	//================添加工具栏三个分类按钮的图片=====================
	_buildingButton = Sprite::create("GameItem/Panel/Building.png");
	_buildingButton->setPosition(Point(-60, 0));
	_buildingButton->setTag(BUILDING_BUTTON);
	addChild(_buildingButton);

	_soldierButton = Sprite::create("GameItem/Panel/Soldier.png");
	_soldierButton->setPosition(Point(0, 0));
	_soldierButton->setTag(SOLDIER_BUTTON);
	addChild(_soldierButton);

	_carButton = Sprite::create("GameItem/Panel/Car.png");
	_carButton->setPosition(Point(60, 0));
	_carButton->setTag(CAR_BUTTON);
	addChild(_carButton);


	//==============添加所有建筑、士兵、战车图标=====================
	addIcons();


	//===============三个标签按钮触摸事件监听===================
	_mainButtonListener = EventListenerTouchOneByOne::create();
	_mainButtonListener->onTouchBegan = [this](Touch* touch, Event* event) {
		Sprite* _selectedButton = static_cast<Sprite*>(event->getCurrentTarget());
		Vec2 locationInNode = _selectedButton->convertToNodeSpace(touch->getLocation());
		Size s = _selectedButton->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		log("listner1 %d %d", s.width, s.height);
		//点击范围判断检测
		if (rect.containsPoint(locationInNode))
		{
			switch (_selectedButton->getTag())
			{
			//================三个标签按钮的点击处理==================
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
				//=======================================================

				return true;
			}
			return false;
		}
	};
	_mainButtonListener->setSwallowTouches(true);   //吞没触摸事件，不向下传递
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener, _buildingButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener->clone(), _soldierButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener->clone(), _carButton);



	//===============图标按钮触摸事件监听===================
	_iconButtonListener = EventListenerTouchOneByOne::create();
	_iconButtonListener->onTouchBegan = [this](Touch* touch, Event* event) {
		Icon* _selectedButton = static_cast<Icon*>(event->getCurrentTarget());
		Vec2 locationInNode = _selectedButton->convertToNodeSpace(touch->getLocation());
		Size s2 = _selectedButton->_iconFrame->getContentSize();
		Rect rect2 = Rect(-s2.width/2, -s2.height/2, s2.width, s2.height);
		log("listner2 %d %d",s2.width,s2.height);
		//点击范围判断检测
		if (rect2.containsPoint(locationInNode))
		{
			log("touch icon");
			switch (_selectedButton->getIconTag())
			{
			//==================建筑类图标的点击处理===================
			case POWER_PLANT_TAG:
			case MINE_TAG:
			case BARRACKS_TAG:
			case CAR_FACTORY_TAG:
				if (_selectedButton->getIsAble())
				{
					_gameScene->_manager->clickCreateBuildingByTag(static_cast<Tag>(_selectedButton->getIconTag()), clock());
					_selectedButton->showProgressOfWait((_gameScene->_manager->getWaitTimeToCreateBuilding())/1000);  //单位转化为秒
				}
				else if (_selectedButton->getStatus() == eIconOK)
				{
					//监听下一次点击的位置坐标，调用manager中的createBuilding()
					_clickToPlaceBuilding = true;
					return true;
				}
				break;
			//=======================================================


			//=================士兵类图标的点击处理====================
			case INFANTRY_TAG:
			case DOG_TAG:
				if (_selectedButton->getIsAble())
				{
					_gameScene->_manager->clickCreateSoldierByTag(static_cast<Tag>(_selectedButton->getIconTag()));
				}
				break;
			//=======================================================

			//=================战车类图标的点击处理====================
			case TANK_TAG:
				if (_selectedButton->getIsAble())
				{
					_gameScene->_manager->clickCreateSoldierByTag(static_cast<Tag>(_selectedButton->getIconTag()));
				}
				break;
			//=======================================================
			}
			return true;
		}
		return false;
	};

	_iconButtonListener->onTouchEnded = [this](Touch* touch, Event* event) {
		if (_clickToPlaceBuilding)
		{
			_gameScene->_manager->createBuilding(touch->getLocation());
			setCurButton(_curCategoryTag);
		}
	};
	_iconButtonListener->setSwallowTouches(true);   //吞没触摸事件，不向下传递
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener, _powerPlantIcon);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener->clone(), _mineIcon);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener->clone(), _barracksIcon);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener->clone(), _carFactoryIcon);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener->clone(), _infantryIcon);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener->clone(), _dogIcon);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_iconButtonListener->clone(), _tankIcon);

	//=================TO DO:监听器的注销===============================

	_buildingList = Vector<Icon*>();
	_soldierList = Vector<Icon*>();
	_carList = Vector<Icon*>();


	scheduleUpdate();

	this->retain();
	this->_powerPlantIcon->retain();
	this->_mineIcon->retain();
	this->_barracksIcon->retain();
	this->_carFactoryIcon->retain();
	this->_infantryIcon->retain();
	this->_dogIcon->retain();
	this->_tankIcon->retain();

	return true;
}

//======================面板的调整===================================
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

	//===============向列表中添加需要显示的图标======================
	switch (tag)
	{
	case BUILDING_BUTTON:
		_buildingList.clear();
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
		_soldierList.clear();
		if (_gameScene->getBarracksNum())
		{
			_soldierList.pushBack(_infantryIcon);
			_soldierList.pushBack(_dogIcon);
		}
		break;
	case CAR_BUTTON:
		_carList.clear();
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
	
	//================设置图标的坐标==================
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
			this->removeChild(_curList->at(i), false);
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


void Panel::update(float dt)
{
	if (_curList != nullptr)
	{
		for (Icon* i : *_curList)
		{
			auto tag = i->getIconTag();
			//图标状态的处理，建筑与Unit方式不同
			switch (tag)
			{
			case POWER_PLANT_TAG:
			case MINE_TAG:
			case BARRACKS_TAG:
			case CAR_FACTORY_TAG:
				if (_gameScene->_manager->_canCreateBuilding && tag == _gameScene->_manager->getBuildingTag())
				{
					i->setStatus(eIconOK);
				}
				else if ((_gameScene->_manager->_isWaitToCreateBuilding || _gameScene->_manager->_canCreateBuilding)
							&& tag != _gameScene->_manager->getBuildingTag())
				{
					i->setStatus(invalidForOtherTask);
				}
				else if (_gameScene->_manager->_isWaitToCreateBuilding && tag == _gameScene->_manager->getBuildingTag())
				{
					i->setStatus(eIconOn);
				}
				else if (i->getMoney() > _gameScene->getMoney())
				{
					i->setStatus(invalidForMoney);

				}
				else
				{
					i->setStatus(eIconPre);
				}

				break;


			case INFANTRY_TAG:
			case DOG_TAG:
				//===========TO DO:与gamescene中的变量名保持一致===========
				if (_gameScene->_manager->_isWaitToCreateSoldier && tag == _gameScene->_manager->getSoldierTag())
				{
					i->setStatus(eIconOnForUnit);
				}
				else if (_gameScene->_manager->_isWaitToCreateSoldier && tag != _gameScene->_manager->getSoldierTag())
				{
					if ((tag == INFANTRY_TAG && _gameScene->getInfantryNum())
						|| (tag == DOG_TAG && _gameScene->getDogNum()))
					{
						i->setStatus(eIconQueuingForUnit);
					}
					else
					{
						i->setStatus(eIconPreForUnit);
					}
					
				}
				else
				{
					i->setStatus(eIconPreForUnit);
				}
				break;

			case TANK_TAG:
				//===========TO DO:与gamescene中的变量名保持一致===========
				if (_gameScene->_manager->_isWaitToCreateCar && tag == _gameScene->_manager->getCarTag())
				{
					i->setStatus(eIconOnForUnit);
				}
				else if (_gameScene->_manager->_isWaitToCreateCar && tag != _gameScene->_manager->getCarTag())
				{
					if (tag == TANK_TAG && _gameScene->getTankNum())
					{
						i->setStatus(eIconQueuingForUnit);
					}
					else
					{
						i->setStatus(eIconPreForUnit);
					}

				}
				else
				{
					i->setStatus(eIconPreForUnit);
				}


				break;
			}
		}
	}
	
}