/*
*  @file     Panel.h
*  @brief    控制栏类，使用控制栏进行建造建筑、训练士兵、制造战车
*  @author   王亮
*/

#ifndef _PANEL_H_
#define _PANEL_H_

#include "cocos2d.h"
#include "Data/Building.h"
#include "Panel/Icon.h"

class GameScene;

class Icon;

class Panel : public cocos2d::Sprite
{
public:
	CC_SYNTHESIZE(int, _numIcon, NumIcon);


	Icon* _powerPlantIcon;
	Icon* _mineIcon;
	Icon* _barracksIcon;
	Icon* _carFactoryIcon;
	Icon* _infantryIcon;
	Icon* _dogIcon;
	Icon* _tankIcon;


private:
	//初始化金钱的消耗
	int sValue[8] = { 500,500,500,500,500,50,50,200 };



	GameScene* _gameScene;

	cocos2d::Sprite* _panelBG;
	cocos2d::Sprite* _buildingButton;
	cocos2d::Sprite* _soldierButton;
	cocos2d::Sprite* _carButton;


	cocos2d::EventListenerTouchOneByOne* _mainButtonListener;


	CC_SYNTHESIZE(Tag, _curCategoryTag, CurCategoryTag);
	cocos2d::Vector<Icon*> _buildingList;
	cocos2d::Vector<Icon*> _soldierList;
	cocos2d::Vector<Icon*> _carList;
	cocos2d::Vector<Icon*>* _curList;

	void showIcon(Tag tag);
	void checkIcon(Tag tag);
	void removeAllIcon();
	void addIcons();

	

  public:
	void setCurButton(Tag tag);

	bool initWithGameScene(GameScene* gameScene);
	static Panel* createWithGameScene(GameScene* gameScene);
      
};

#endif