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
	void update(float dt);
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
	int sValue[8] = { 500,500,500,500,50,50,200 };

	bool _clickToPlaceBuilding;
	int _canToBuild = 1;
	GameScene* _gameScene;

	cocos2d::Sprite* _panelBG;
	cocos2d::Sprite* _buildingButton;
	cocos2d::Sprite* _soldierButton;
	cocos2d::Sprite* _carButton;


	cocos2d::EventListenerTouchOneByOne* _mainButtonListener;
	cocos2d::EventListenerTouchOneByOne* _iconButtonListener;

	bool canBuild(cocos2d::Vec2 v, GameScene *_gameScene);

	CC_SYNTHESIZE(Tag, _curCategoryTag, CurCategoryTag);
	cocos2d::Vector<Icon*> _buildingList;
	cocos2d::Vector<Icon*> _soldierList;
	cocos2d::Vector<Icon*> _carList;
	cocos2d::Vector<Icon*>* _curList;

	/**
	* @brief 设置图标坐标并显示
	* @param Tag
	* @return void
	*/
	void showIcon(Tag tag);

	/**
	* @brief 更新列表中应显示的图标
	* @param Tag
	* @return void
	*/
	void checkIcon(Tag tag);

	/**
	* @brief 移除当前列表中的全部图标
	* @return void
	*/
	void removeAllIcon();

	/**
	* @brief 初始化所有图标
	* @return void
	*/
	void addIcons();



public:
	void setCurButton(Tag tag);

	bool initWithGameScene(GameScene* gameScene);
	static Panel* createWithGameScene(GameScene* gameScene);

};

#endif