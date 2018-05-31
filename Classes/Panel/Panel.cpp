/*
*  @file     Panel.cpp
*  @brief    控制栏类，使用控制栏进行建造建筑、训练士兵、制造战车
*  @author   王亮
*/

#include "cocos2d.h"
#include "Panel/Panel.h"
#include "Panel/Icon.h"

//#include "Manager/GameManager.h"

USING_NS_CC;

class GameScene;

bool Panel::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	//TO DO:添加工具栏背景图片
	_panelBG = Sprite::create("GameItem/Panel/panelBG_normal.png");
	_panelBG->setPosition(Point(0, 0));
	addChild(_panelBG);

	_curCategoryTag = BUILDING_BUTTON;

	//TO DO:添加工具栏三个分类按钮的图片
	_buildingButton = Sprite::create("GameItem/Panel/testIcon.png");
	_buildingButton->setPosition(Point(-60,0));
	_buildingButton->setTag(BUILDING_BUTTON);
	addChild(_buildingButton);

	_soldierButton = Sprite::create("GameItem/Panel/testIcon.png");
	_soldierButton->setPosition(Point(0,0));
	_soldierButton->setTag(SOLDIER_BUTTON);
	addChild(_soldierButton);

	_carButton = Sprite::create("GameItem/Panel/testIcon.png");
	_carButton->setPosition(Point(60,0));
	_carButton->setTag(CAR_BUTTON);
	addChild(_carButton);


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

	_buildingList = Vector<Sprite*>();
	_soldierList = Vector<Sprite*>();
	_carList = Vector<Sprite*>();

	return true;
}


//TO DO:面板的调整
void Panel::setCurButton(Tag tag)
{


}

void Panel::addIcons()
{

}