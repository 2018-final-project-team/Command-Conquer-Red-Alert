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

	_curCategory = BUILDING;

	//TO DO:添加工具栏三个分类按钮的图片
	_buildingButton = Sprite::create("GameItem/Panel/testIcon.png");
	_buildingButton->setPosition(Point(-60,0));
	addChild(_buildingButton);
	_soldierButton = Sprite::create("GameItem/Panel/testIcon.png");
	_soldierButton->setPosition(Point(0,0));
	addChild(_soldierButton);
	_carButton = Sprite::create("GameItem/Panel/testIcon.png");
	_carButton->setPosition(Point(60,0));
	addChild(_carButton);

	//===============注册监听器===================
	_mainButtonListener = EventListenerTouchOneByOne::create();
    _mainButtonListener->onTouchBegan = [=](Touch* touch, Event* event) {
		Sprite* _selectedButton = static_cast<Sprite*>(event->getCurrentTarget());
		switch (_selectedButton->getTag())
		{
		case BUILDING:
			setCurButton(BUILDING);
			break;
		case SOLDIER:
			setCurButton(SOLDIER);
			break;
		case CAR:
			setCurButton(CAR);
			break;
		}
        
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_mainButtonListener, this);


	_buildingList = Vector<Sprite*>();
	_soldierList = Vector<Sprite*>();
	_carList = Vector<Sprite*>();

	return true;
}

//TO DO:面板的调整
void Panel::setCurButton(Category cat)
{


}

void Panel::addIcons()
{

}


		// auto test = Sprite::create("mainmenu_saveslot_confirmdelete_0001.png");
		// auto size = logo->getContentSize();
		// test->setPosition(size.width / 2, size.height / 2);
		// logo->addChild(test);