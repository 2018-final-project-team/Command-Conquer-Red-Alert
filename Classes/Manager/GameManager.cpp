/*
*  @file     GameManager.cpp
*  @brief    游戏的控制类
*/

#include "GameManager.h"
#include "../Data/BuildingData.hpp"

USING_NS_CC;

Manager* Manager::create(GameScene* gameScene)
{
    Manager* manager = new Manager();
    if (manager)
    {
        manager->setGameScene(gameScene);
        manager->setMoveController(gameScene);
        manager->setDatas();
        manager->autorelease();
        return manager;
    }

    CC_SAFE_DELETE(manager);
    return nullptr;
}

void Manager::setGameScene(GameScene* gameScene)
{
    _gameScene = gameScene;
}

void Manager::setMoveController(GameScene* gameScene)
{
    _moveController = MoveController::create(gameScene);
}

void Manager::setDatas()
{
    _waitToCreateBuilding = _waitToCreateSoldier = false;
    _canCreateBuilding = _canCreateSoldier = false;
    _enemy = _building = nullptr;
}

void Manager::clickCreateBuildingByTag(Tag building_tag, clock_t start_time)
{
    if (!_waitToCreateBuilding)
    {
        int castMoney, castPower;
        switch (building_tag)
        {
        case POWER_PLANT_TAG:
            castMoney = powerPlantCastMoney;       //  in BuildingData.h
            castPower = powerPlantCastPower;  
            _waitToCreateBuilding = powerPlantWait;
            break;
        case MINE_TAG:
            castMoney = mineCastMoney;
            castPower = mineCastPower;
            _waitToCreateBuilding = mineWait;
            break;
        case BARRACKS_TAG:
            castMoney = barrackCastMoney;
            castPower = barrackCastPower;
            _waitToCreateBuilding = barrackWait;
            break;
        case CAR_FACTORY_TAG:
            castMoney = carFactoryCastMoney;
            castPower = carFactoryCastPower;
            _waitToCreateBuilding = carFactoryWait;
            break;
        }

        if (_gameScene->getMoney() >= castMoney &&
            _gameScene->getPower() >= castPower)
        {
            _waitToCreateBuilding = true;
            _timeToCreateBuilding = start_time;
            _buildingTag = building_tag;
            _gameScene->decreaseMoney(castMoney);
            _gameScene->decreasePower(castPower);
        }
    }
}

void Manager::clickCreateSoldierByTag(Tag soldier_tag, clock_t start_time)
{
    if (!_waitToCreateSoldier )
    {
        int castMoney, castPower;
        switch (soldier_tag)
        {
        case INFANTRY_TAG:
            castMoney = infantryCastMoney;
            castPower = infantryCastPower;
            if (!_gameScene->getHaveBarracks())
            {
                return;
            }
            break;
        case DOG_TAG:
            castMoney = dogCastMoney;
            castPower = dogCastPower;
            if (!_gameScene->getHaveBarracks())
            {
                return;
            }
            break;
        case TANK_TAG:
            castMoney = tankCastMoney;
            castPower = tankCastPower;
            if (!_gameScene->getHaveCarFactory())
            {
                return;
            }
            break;
        }

        if (_gameScene->getMoney() >= castMoney &&
            _gameScene->getPower() >= castPower)
        {
            _waitToCreateSoldier = true;
            _timeToCreateSoldier = start_time;
            _soldierTag = soldier_tag;
            _gameScene->decreaseMoney(castMoney);
            _gameScene->decreasePower(castPower);
        }
    }
}

void Manager::waitCreateBuilding()
{
    if (_waitToCreateBuilding)
    {
        if (clock() - _timeToCreateBuilding > _waitToCreateBuilding)
        {
            _canCreateBuilding = true;
        }
    }
}

void Manager::waitCreateSoldier()
{
    if (_waitToCreateSoldier)
    {
        if (clock() - _timeToCreateSoldier > _waitToCreateSoldier)
        {
            _canCreateSoldier = true;
        }
    }
}

void Manager::createBuilding(cocos2d::Vec2 position)
{
    if (_canCreateBuilding)
    {
        Building* building = Building::createByTag(_buildingTag, position);
        switch (_buildingTag)
        {
        case POWER_PLANT_TAG:
            _gameScene->setHavePowerPlant(true);
            break;
        case MINE_TAG:
            _gameScene->setHaveMine(true);
            break;
        case BARRACKS_TAG:
            _gameScene->setHaveBarracks(true);
            break;
        case CAR_FACTORY_TAG:
            _gameScene->setHaveCarFactory(true);
            break;
        }
        _gameScene->getBuildings().pushBack(building);
        _canCreateBuilding = false;
        _waitToCreateBuilding = false;
    }
}

void Manager::createSoldier()
{
    if (_canCreateSoldier)
    {
        Soldier* soldier = Soldier::createByTag(_soldierTag);
        _gameScene->getSoldiers().pushBack(soldier);
        _canCreateSoldier = false;
        _waitToCreateSoldier = false;
    }
}

void Manager::setEnemy(Soldier* enemy)
{
    _enemy = enemy;
    _building = nullptr;
}

void Manager::setBuilding(Building* building)
{
    _building = building;
    _enemy = nullptr;
}

void Manager::attack()
{

}