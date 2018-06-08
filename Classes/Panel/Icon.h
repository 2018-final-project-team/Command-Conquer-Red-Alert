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

//设置8种状态：由于金钱不足而无法点击，由于其他任务正在进行而无法点击，可点击并进入倒计时，
//倒计时中，计时结束，用于Unit的可点击，用于Unit的计时中，任务累积排队中
//建筑类使用5种：1~5
//Unit类使用4种：6~8
enum IconsStatus
{
	invalidForMoney, 
	invalidForOtherTask, 
	eIconPre,
	eIconOn,
	eIconOK,

	eIconPreForUnit,
	eIconOnForUnit,
	eIconQueuingForUnit

};

class GameScene;

class Icon : public cocos2d::Sprite
{
public:
	bool initIcon(Tag tag,int money, GameScene* gameScene);
	static Icon* createIcon(Tag tag,int money, GameScene* gameScene);

	/**
	* @brief 修改图标的status和显示效果
	* @param 状态
	* @return void
	*/
	void setStatus(IconsStatus iconSta);

	IconsStatus getStatus();

	/**
	* @brief 半径模式显示图标任务的百分比进度，在manager中调用
	* @param 任务耗时，单位为秒
	* @return void
	*/
	void showProgressOfWait(float duration);

private:
	GameScene* _gameScene;

	CC_SYNTHESIZE(bool, _isAble, IsAble);   //是否可通过点击增加任务
	CC_SYNTHESIZE(Tag, tag, tag);         //图标的Tag
    CC_SYNTHESIZE(int, money, Money);    //执行建造or训练or制造计划需要的金钱数

	cocos2d::ProgressTimer* progressTimer1;
	cocos2d::ProgressTimer* progressTimer2;

    
	cocos2d::Sprite* _icon;           //内层显示的图标
	cocos2d::Sprite* _iconFrame;       //最外层框图
	cocos2d::Sprite* _priceIcon;      //价格图标
	cocos2d::Label* _priceLabel;      //价格label
	cocos2d::Label* _statusLabel;      //状态label
	cocos2d::Label* _statusLabel2;      //状态label2，当一个状态label不够用时使用
	cocos2d::Sprite* _invalidIcon;     //不可点击效果的图标

	IconsStatus _status;

};

#endif