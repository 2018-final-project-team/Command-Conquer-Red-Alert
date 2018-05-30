#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

const clock_t addMoneyDelay = 1000 * 10;

#include <time.h>
#include "cocos2d.h"
#include "Data\UnitData.h"
#include "Manager\GameManager.h"
#include "Panel/Panel.h"

class Manager;   //解决头文件互相包含时带来的问题

class GameScene : public cocos2d::Layer
{
private:
	Panel* panel;
	cocos2d::TMXTiledMap* _tileMap;
	cocos2d::TMXLayer* _ground;
	cocos2d::Point _cursorPosition{ 0,0 };  // C++ 11 允许这样初始化
	void scrollMap();

	cocos2d::Vector<Unit*> _selectedSoldiers;
	cocos2d::Vector<Unit*> _enemySoldiers;
	cocos2d::Vector<Unit*> _soldiers;
	cocos2d::Vector<Building*> _buildings;

	Point _touchBegan;
	Point _touchEnd;
	Manager* _manager;

public:
	EventListenerTouchOneByOne* _gameListener;
	EventDispatcher* _gameEventDispatcher;

public:
	CC_SYNTHESIZE(int, _money, Money);

	// 总电力
	CC_SYNTHESIZE(int, _totalPower, TotalPower);

	// 剩余电力
	CC_SYNTHESIZE(int, _power, Power);

	// 电力是否足够
	CC_SYNTHESIZE(bool, _isPowerEnough, IsPowerEnough);

	// 是否有基地
	CC_SYNTHESIZE(bool, _isBaseExist, IsBaseExist);

	CC_SYNTHESIZE(int, _barracksNum, BarracksNum);

	CC_SYNTHESIZE(int, _mineNum, MineNum);

	CC_SYNTHESIZE(int, _powerPlantNum, PowerPlantNum);

	CC_SYNTHESIZE(int, _carFactoryNum, CarFactoryNum);

	CC_SYNTHESIZE(cocos2d::Vec2, _carFactoryPosition, CarFactoryPosition);

	CC_SYNTHESIZE(cocos2d::Vec2, _barracksPosition, BarracksPosition);

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onExit();

	// 初始化数据
	void dataInit();

	virtual void update(float time);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuBackCallback(Ref *pSender);

	/**
	* @brief getSelectedSoldiers
	* @return the address of _selectedSoldiers
	*/
	cocos2d::Vector<Unit*>* getSelectedSoldiers();

	/**
	* @brief getSoldiers
	* @return the address of _soldiers
	*/
	cocos2d::Vector<Unit*>* getSoldiers();

	/**
	* @brief getBuildings
	* @return the address of _buildings
	*/
	cocos2d::Vector<Building*>* getBuildings();

	/**
	* @brief addMoney
	* @param 增加的钱
	* @return void
	*/
	void addMoney(int money);

	/**
	* @brief decreaseMoney
	* @param 减少的钱
	* @return void
	*/
	void decreaseMoney(int money);

	/**
	* @brief addPower 增加电
	* @return void
	*/
	void addPower(int power);

	/**
	* @brief decreasePower
	* @return void
	*/
	void decreasePower(int power);

	/**
	* @brief addTotalPower 增加总电
	* @return void
	*/
	void addTotalPower(int power);

	/**
	* @brief decreaseTotalPower
	* @return void
	*/
	void decreaseTotalPower(int power);

	/*
	* @brief 电厂数量加一
	* @return void
	*/
	void addPowerPlant() { _powerPlantNum++; }

	/*
	* @brief 电厂数量减一
	* @return void
	*/
	void decreasePowerPlant() { _powerPlantNum--; }

	/*
	* @brief 车厂数量加一
	* @return void
	*/
	void addCarFactory() { _carFactoryNum++; }

	/*
	* @brief 车厂数量减一
	* @return void
	*/
	void decreaseCarFactory() { _carFactoryNum--; }

	/*
	* @brief 兵营数量加一
	* @return void
	*/
	void addBarracks() { _barracksNum++; }

	/*
	* @brief 兵营数量减一
	* @return void
	*/
	void decreaseBarracks() { _barracksNum--; }

	/*
	* @brief 矿厂数量加一
	* @return void
	*/
	void addMine() { _mineNum++; }

	/*
	* @brief 矿产数量减一
	* @return void
	*/
	void decreaseMine() { _mineNum--; }

	/*
	* @brief isCollision
	* @param the position
	* @return bool
	*/
	bool isCollision(cocos2d::Vec2 position);

	/*
	* @brief getTileSize
	* @return the size of tile
	*/
	float getTileSize();

	/*
	* @brief getEnemySoldiers
	* @return the address of enemy_soldiers
	*/
	//To Do:和网络也许有关系
	cocos2d::Vector<Unit*> * getEnemySoldiers() { return &_enemySoldiers; }

};

#endif // __Welcome_SCENE_H__