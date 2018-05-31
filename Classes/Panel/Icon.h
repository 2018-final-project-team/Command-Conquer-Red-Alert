/*
*  @file     Icon.h
*  @brief    控制图标类，控制栏中显示的图标
*  @author   王亮
*/

#ifndef _ICON_H_
#define _ICON_H_

#include "cocos2d.h"
#include "Data/TagData.h"
#include "Scene/GameScene.h"

//设置三种状态：可点击并进入倒计时，倒计时中，计时结束
enum IconsStatus{eIconPre,eIconOn,eIconOK};

USING_NS_CC;

class GameScene;

class Icon : public Sprite
{
public:
	bool initIcon(Tag tag,int money, GameScene* gameScene);
	static Icon* createIcon(Tag tag,int money, GameScene* gameScene);

    //根据iconSta修改精灵状态
	void setStatus(IconsStatus iconSta);

	void setSelected();           //设置图标被点击，进入倒计时状态
	void setNotSelected();        //设置图标为初始可点击的状态
	void setNotEnoughMoney();     //这是由于金币不足导致无法点击的状态



private:
	GameScene* _gameScene;

	CC_SYNTHESIZE(bool, isAble, IsAble);   //是否可点击
	CC_SYNTHESIZE(Tag, tag, tag);         //图标的Tag
	CC_SYNTHESIZE(bool, isSelected, IsSelected);   //是否被选中
    CC_SYNTHESIZE(int, money, Money);    //执行建造or训练or制造计划需要的金钱数
	void update(float dt);

    
	Sprite* _icon;           //内层显示的图标
	Sprite* _iconFrame;       //最外层框图
	Sprite* _okIcon;         //计时完毕的图标
	Sprite* _priceIcon;      //价格图标
	Label* _priceLabel;      //价格label
};

#endif