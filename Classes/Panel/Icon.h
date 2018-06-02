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

//设置5种状态：由于金钱不足而无法点击，由于其他任务正在进行而无法点击，可点击并进入倒计时，倒计时中，计时结束
enum IconsStatus{invalidForMoney, invalidForOtherTask, eIconPre,eIconOn,eIconOK};

USING_NS_CC;

class GameScene;

class Icon : public Sprite
{
public:
	bool initIcon(Tag tag,int money, GameScene* gameScene);
	static Icon* createIcon(Tag tag,int money, GameScene* gameScene);

    //修改图标的status和显示效果
	void setStatus(IconsStatus iconSta);

	/**
	* @brief 半径模式显示图标任务的百分比进度，在manager中调用
	* @param 任务耗时，单位为秒
	* @return void
	*/
	void showProgressOfWait(float duration);



private:
	GameScene* _gameScene;

	CC_SYNTHESIZE(bool, _isSelected, IsSelected);
	CC_SYNTHESIZE(Tag, tag, tag);         //图标的Tag
    CC_SYNTHESIZE(int, money, Money);    //执行建造or训练or制造计划需要的金钱数

    
	Sprite* _icon;           //内层显示的图标
	Sprite* _iconFrame;       //最外层框图
	Sprite* _priceIcon;      //价格图标
	Label* _priceLabel;      //价格label
	Sprite* _statusIcon;      //状态图标
	Label* _statusLabel;      //状态label
	Sprite* _invalidIcon;     //不可点击效果的图标

	IconsStatus _status;

};

#endif