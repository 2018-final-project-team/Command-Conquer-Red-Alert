#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"
#include "Manager\GameManager.h"
#include "Data\Building.h"
#include "Data\UnitData.h"

typedef enum
{
    BASE_TAG,
    POWER_PLANT_TAG,
    MINE_TAG,
    BARRACKS_TAG,
    CAR_FACTORY_TAG,
    INFANTRY_TAG,
    DOG_TAG,
    TANK_TAG
}Tag;

class GameScene : public cocos2d::Layer
{
private:
    cocos2d::Vector<Unit*> _selectedSoldiers;
    cocos2d::Vector<Unit*> _soldiers;
    cocos2d::Vector<Building*> _buildings;

    Manager* manager;

public:
    CC_SYNTHESIZE(int, _money, Money);

    CC_SYNTHESIZE(int, _power, Power);

    CC_SYNTHESIZE(int, _barracksNum, BarracksNum);

    CC_SYNTHESIZE(int, _mineNum, MineNum);

    CC_SYNTHESIZE(int, _powerPlantNum, PowerPlantNum);

    CC_SYNTHESIZE(int, _carFactoryNum, CarFactoryNum);

    CC_SYNTHESIZE(cocos2d::Vec2, _carFactoryPosition, CarFactoryPosition);

    CC_SYNTHESIZE(cocos2d::Vec2, _barracksPosition, BarracksPosition);

	static cocos2d::Scene* createScene();

	virtual bool init();

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
    * @param Ôö¼ÓµÄÇ®
    * @return void
    */
    void addMoney(int money);

    /**
    * @brief decreaseMoney
    * @param ¼õÉÙµÄÇ®
    * @return void
    */
    void decreaseMoney(int money);

    /**
    * @brief addPower Ôö¼Ó100µç
    * @return void
    */
    void addPower();

    /**
    * @brief decreasePower
    * @param ¼õÉÙµÄµç
    * @return void
    */
    void decreasePower(int power);

    /*
    * @brief µç³§ÊýÁ¿¼ÓÒ»
    * @return void
    */
    void addPowerPlant() { _powerPlantNum++; }

    /*
    * @brief µç³§ÊýÁ¿¼õÒ»
    * @return void
    */
    void decreasePowerPlant() { _powerPlantNum--; }

    /*
    * @brief ³µ³§ÊýÁ¿¼ÓÒ»
    * @return void
    */
    void addCarFactory() { _carFactoryNum++; }

    /*
    * @brief ³µ³§ÊýÁ¿¼õÒ»
    * @return void
    */
    void decreaseCarFactory() { _carFactoryNum--; }

    /*
    * @brief ±øÓªÊýÁ¿¼ÓÒ»
    * @return void
    */
    void addBarracks() { _barracksNum++; }

    /*
    * @brief ±øÓªÊýÁ¿¼õÒ»
    * @return void
    */
    void decreaseBarracks() { _barracksNum--; }

    /*
    * @brief ¿ó³§ÊýÁ¿¼ÓÒ»
    * @return void
    */
    void addMine() { _mineNum++; }

    /*
    * @brief ¿ó²úÊýÁ¿¼õÒ»
    * @return void
    */
    void decreaseMine() { _mineNum--; }

};

#endif // __Welcome_SCENE_H__