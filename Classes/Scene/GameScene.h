#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

const clock_t addMoneyDelay = 1000 * 10;

#include <time.h>
#include "cocos2d.h"
#include "Data\UnitData.h"
#include "Manager\GameManager.h"
#include "Panel/Panel.h"

class Manager;   //解决头文件互相包含时带来的问题
class Panel;     //解决头文件互相包含时带来的问题

class GameScene : public cocos2d::Layer
{
public:
    Manager * _manager;
    cocos2d::TMXTiledMap* _tileMap;

	Panel * panel;

private:
	cocos2d::TMXLayer* _barrier;
    int MAPX;
    int MAPY;

	cocos2d::Point _cursorPosition{ 0,0 };  // C++ 11 允许这样初始化
	void scrollMap();

	cocos2d::Vector<Unit*> _selectedSoldiers;

	cocos2d::Vector<Unit*> _enemySoldiers;
	cocos2d::Vector<Unit*> _soldiers;
	cocos2d::Vector<Building*> _buildings;
    cocos2d::Vector<Building*> _enemyBuildings;

	cocos2d::Point _touchBegan;
	cocos2d::Point _touchEnd;

public:
	cocos2d::EventListenerTouchOneByOne* _gameListener;
	cocos2d::EventDispatcher* _gameEventDispatcher;

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

    // 待造坦克数
    CC_SYNTHESIZE(int, _tankNum, TankNum);
    // 待造狗数
    CC_SYNTHESIZE(int, _dogNum, DogNum);
    // 待造步兵数
    CC_SYNTHESIZE(int, _infantryNum, InfantryNum);

	CC_SYNTHESIZE(cocos2d::Vec2, _carFactoryPosition, CarFactoryPosition);

	CC_SYNTHESIZE(cocos2d::Vec2, _barracksPosition, BarracksPosition);

	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnter();
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

    /*
    * @brief getEnemySoldiers
    * @return the address of enemy_soldiers
    */
    cocos2d::Vector<Unit*> * getEnemySoldiers() { return &_enemySoldiers; }

	/**
	* @brief getBuildings
	* @return the address of _buildings
	*/
	cocos2d::Vector<Building*>* getBuildings();

    /*
    * @brief getEnemyBuildings
    * @return the address of enemy_soldiers
    */
    cocos2d::Vector<Building*> * getEnemyBuildings() { return &_enemyBuildings; }

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
    * @brief 待造狗加一
    */
    void addDog() { _dogNum++; }

    /*
    * @brief 待造兵加一
    */
    void addInfantry() { _infantryNum++; }

    /*
    * @brief 待造坦克加一
    */
    void addTank() { _tankNum++; }

    /*
    * @brief 待造狗减一
    */
    void decreaseDog() { _dogNum--; }

    /*
    * @brief 待造兵减一
    */
    void decreaseInfantry() { _infantryNum--; }

    /*
    * @brief 待造坦克减一
    */
    void decreaseTank() { _tankNum--; }

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
    *@brief 移动所有士兵建筑 包括目的地
    */
    void moveSpritesWithMap(cocos2d::Vec2 direction);

};

#endif // __Welcome_SCENE_H__
