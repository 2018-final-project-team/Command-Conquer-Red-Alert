/**
*  @file     GameManager.h
*  @brief    游戏的控制类
**/

#ifndef __GameManager_H_
#define __GameManager_H_

#include "cocos2d.h"
#include "MoveController.h"
#include <queue>
#include <iostream>
#include <time.h>

class GameScene;              //解决头文件互相包含时带来的问题

class MoveController;         //解决头文件互相包含时带来的问题

class Manager : public cocos2d::Node
{
public:
	bool _isWaitToCreateBuilding;
	bool _canCreateBuilding;
	bool _isWaitToCreateSoldier;
	bool _isWaitToCreateCar;
private:

	GameScene* _gameScene;
	MoveController* _moveController;

	std::queue<Tag> _carQueue;               // 待建车队列
	std::queue<Tag> _soldierQueue;           // 待建士兵队列 

											 //========CreateController===========
	CC_SYNTHESIZE(clock_t, _timeToCreateBuilding, TimeToCreateBuilding);               // 单位毫秒
	CC_SYNTHESIZE(clock_t, _waitTimeToCreateBuilding, WaitTimeToCreateBuilding);
	CC_SYNTHESIZE(clock_t, _timeToCreateSoldier, TimeToCreateSoldier);
	CC_SYNTHESIZE(clock_t, _waitTimeToCreateSoldier, WaitTimeToCreateSoldier);
	CC_SYNTHESIZE(clock_t, _timeToCreateCar, TimeToCreateCar);
	CC_SYNTHESIZE(clock_t, _waitTimeToCreateCar, WaitTimeToCreateCar);

	CC_SYNTHESIZE(Tag, _buildingTag, BuildingTag);
	CC_SYNTHESIZE(Tag, _soldierTag, SoldierTag);
	CC_SYNTHESIZE(Tag, _carTag, CarTag);

	//===================================

	//==========Attack================
	Unit* _selectedEnemy;              //被选中的士兵或建筑
	Building* _selectedBuilding;

public:
	/**
	* @brief Manager的静态构造函数
	* @return  Manager*
	*/
	static Manager* createWithGameScene(GameScene* gameScene);

	/**
	* @brief initWithGameScene
	* @return bool
	*/
	bool initWithGameScene(GameScene* gameScene);

	bool getCanCreateBuilding() { return _canCreateBuilding; }

	//=====================CreateController========================
	/**
	* @brief 点击建造建筑的开始
	* @param1 建筑的Tag
	* @param2 开始等待的时间
	* @return  void
	*/
	void clickCreateBuildingByTag(Tag building_tag, clock_t start_time);

	/**
	* @brief 点击建造士兵的开始
	* @param1 士兵的Tag
	* @return  void
	*/
	void clickCreateSoldierByTag(Tag soldier_tag);

	/**
	* @brief 建造建筑的等待 需要在update函数里调用
	* @return  void
	*/
	void waitCreateBuilding();

	/**
	* @brief 建造士兵的等待 需要在update函数里调用
	* @return  void
	*/
	void waitCreateSoldier();

	/**
	* @brief 建造车的等待 需要在update函数里调用
	* @return  void
	*/
	void waitCreateCar();

	/**
	* @brief 建造建筑 在GameScene中调用
	* @pamra 造建筑点
	* @return  void
	*/
	void createBuilding(cocos2d::Vec2 position);

	/**
	* @brief 重新计算电量
	* @return  void
	*/
	void resetPower();

	//============================================================

	//====================attack============================

	void setEnemy(Unit* enemy);
	void setBuilding(Building* building);

	/**
	* @brief 攻击 在Update函数里调用
	*/
	void attack();

	/**
	* @brief 加钱 在Update函数里调用
	*/
	void addMoneyUpdate();

	/*
	* @brief get _moveController
	* @return the  _movecontroller( a point )
	*/
	MoveController* getMoveController()
	{
		return _moveController;
	}

	// 放士兵和坦克
	cocos2d::Point getPutSoldierPosition();

	cocos2d::Point getPutCarPosition();

};

#endif // __GameManeger_H_
