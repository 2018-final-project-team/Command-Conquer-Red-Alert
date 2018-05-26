/*
*  @file     GameManager.cpp
*  @brief    游戏的控制类
*/

#include "GameManager.h"

USING_NS_CC;

Manager* Manager::createWithGameScene(GameScene* gameScene)
{
    Manager* manager = new Manager();
    if (manager -> initWithGameScene(gameScene))
    {
        manager->autorelease();
        return manager;
    }

    CC_SAFE_DELETE(manager);
    return nullptr;
}

bool Manager::initWithGameScene(GameScene* gameScene)
{
    _gameScene = gameScene;
    _moveController = MoveController::createWithGameScene(gameScene);

    _waitToCreateBuilding = _waitToCreateSoldier = false;
    _canCreateBuilding = false;
    _selectedEnemy = nullptr;
    _selectedBuilding = nullptr;
}

void Manager::clickCreateBuildingByTag(Tag building_tag, clock_t start_time)
{
    if (!_waitToCreateBuilding)
    {
        int castMoney, castPower;
        switch (building_tag)
        {
        case POWER_PLANT_TAG:
            castMoney = buildingData::powerPlantCastMoney;       //  in BuildingData.h
            castPower = buildingData::powerPlantCastPower;
            _waitToCreateBuilding = buildingData::powerPlantWait;
            break;
        case MINE_TAG:
            castMoney = buildingData::mineCastMoney;
            castPower = buildingData::mineCastPower;
            _waitToCreateBuilding = buildingData::mineWait;
            break;
        case BARRACKS_TAG:
            castMoney = buildingData::barracksCastMoney;
            castPower = buildingData::barracksCastPower;
            _waitToCreateBuilding = buildingData::barracksWait;
            break;
        case CAR_FACTORY_TAG:
            castMoney = buildingData::carFactoryCastMoney;
            castPower = buildingData::carFactoryCastPower;
            _waitToCreateBuilding = buildingData::carFactoryWait;
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
            if (_gameScene->getBarracksNum() == 0)
            {
                return;
            }
            castMoney = unitData::infantryCastMoney;
            castPower = unitData::infantryCastPower;
            break;
        case DOG_TAG:
            if (_gameScene->getBarracksNum() == 0)
            {
                return;
            }
            castMoney = unitData::dogCastMoney;
            castPower = unitData::dogCastPower;
            break;
        case TANK_TAG:
            if (_gameScene->getCarFactoryNum() == 0)
            {
                return;
            }
            castMoney = unitData::tankCastMoney;
            castPower = unitData::tankCastPower;
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
            Unit* soldier = Unit::create(_soldierTag);
            if (_soldierTag == TANK_TAG)
            {
                soldier->setPosition(_gameScene->getCarFactoryPosition());
            }
            else
            {
                soldier->setPosition(_gameScene->getBarracksPosition());
            }
            _gameScene->addChild(soldier);
            _gameScene->getSoldiers()->pushBack(soldier);
            _waitToCreateSoldier = false;
        }
    }
}

void Manager::createBuilding(cocos2d::Vec2 position)
{
    if (_canCreateBuilding)
    {
        Building* building = Building::create(_buildingTag);
        building->setPosition(position);
        _gameScene->addChild(building);
        switch (_buildingTag)
        {
        case POWER_PLANT_TAG:
            _gameScene->addPowerPlant();            // 电厂数量加一
            _gameScene->addPower();                 // 加100电量                 
            break;
        case MINE_TAG:
            _gameScene->addMine();                  // 矿场数量加一
            break;
        case BARRACKS_TAG:
            _gameScene->addBarracks();
            _gameScene->setBarracksPosition(building->getPosition());  // 兵营位置
            break;
        case CAR_FACTORY_TAG:
            _gameScene->addCarFactory();
            _gameScene->setCarFactoryPosition(building->getPosition());
            break;
        }
        _gameScene->getBuildings()->pushBack(building);
        _canCreateBuilding = false;
        _waitToCreateBuilding = false;
    }
}

void Manager::setEnemy(Unit* enemy)
{
    _selectedEnemy = enemy;
    _selectedBuilding = nullptr;
}

void Manager::setBuilding(Building* building)
{
    _selectedBuilding = building;
    _selectedEnemy = nullptr;
}

void Manager::attack()
{
    static clock_t infantryPreT = clock();
    static clock_t dogPreT = clock();
    static clock_t tankPreT = clock();
    bool isInfantryAttack = false;
    bool isDogAttack = false;
    bool isTankAttack = false;
    bool isBuildingDied = false;
    Tag selectedBuildingTag;
    Vector<Unit*>& enemySoldiers = _gameScene->getEnemySoldiers();
    clock_t nowT = clock();

    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (_selectedEnemy && soldier->canAttack(_selectedEnemy->getPosition()))    // 如果选中了敌方士兵
        {
            switch (soldier->getTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitCD())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->setDeath();
                        enemySoldiers.eraseObject(_selectedEnemy);
                        _gameScene->removeChild(_selectedEnemy);
                        _selectedEnemy = nullptr;
                    }
                    isInfantryAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitCD())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->setDeath();
                        enemySoldiers.eraseObject(_selectedEnemy);
                        _gameScene->removeChild(_selectedEnemy);
                        _selectedEnemy = nullptr;
                    }
                    isDogAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitCD())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->setDeath();
                        enemySoldiers.eraseObject(_selectedEnemy);
                        _gameScene->removeChild(_selectedEnemy);
                        _selectedEnemy = nullptr;
                    }
                    isTankAttack = true;
                }
                break;
            }

            continue;
        }
        else if (_selectedBuilding && soldier->canAttack(_selectedBuilding->getPosition()))  // 如果选中了敌方建筑
        {
            switch (soldier->getTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitCD())
                {
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        selectedBuildingTag = _selectedBuilding->getBuildingTag();
                        _gameScene->getBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                    }
                    isInfantryAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitCD())
                {
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        selectedBuildingTag = _selectedBuilding->getBuildingTag();
                        _gameScene->getBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        isBuildingDied = true;
                    }
                    isDogAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitCD())
                {
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        selectedBuildingTag = _selectedBuilding->getBuildingTag();
                        _gameScene->getBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        isBuildingDied = true;
                    }
                    isTankAttack = true;
                }
            }

            continue;
        }

        for (auto& enemy : enemySoldiers)
        {
            if (soldier->canAttack(enemy->getPosition()))
            {
                switch (soldier->getTag())
                {
                case INFANTRY_TAG:
                    if (nowT - infantryPreT >= soldier->getUnitCD())
                    {
                        soldier->attack(enemy);
                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers.eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isInfantryAttack = true;
                    }
                    break;
                case DOG_TAG:
                    if (nowT - dogPreT >= soldier->getUnitCD())
                    {
                        soldier->attack(enemy);
                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers.eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isDogAttack = true;
                    }
                    break;
                case TANK_TAG:
                    if (nowT - tankPreT >= soldier->getUnitCD())
                    {
                        soldier->attack(enemy);
                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers.eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isTankAttack = true;
                    }
                    break;
                }
                break;
            }
        }
    }

    if (isInfantryAttack)
    {
        infantryPreT = nowT;
    }
    if (isDogAttack)
    {
        dogPreT = nowT;
    }
    if (isTankAttack)
    {
        tankPreT = nowT;
    }

    if (isBuildingDied)
    {
        switch (selectedBuildingTag)
        {
        case POWER_PLANT_TAG:
            _gameScene->decreasePowerPlant();            // 电厂数量减一 如果电厂数量为0
            if (_gameScene->getPowerPlantNum() == 0)                 // 电量为0    
            {
                _gameScene->setPower(0);
            }
            break;
        case MINE_TAG:
            _gameScene->decreaseMine();                  // 矿场数量-1
            break;
        case BARRACKS_TAG:
            _gameScene->decreaseBarracks();         
            for (auto& building : *(_gameScene->getBuildings()))
            {
                if (building->getBuildingTag() == BARRACKS_TAG)
                {
                    _gameScene->setBarracksPosition(building->getPosition());
                }
            }
            break;
        case CAR_FACTORY_TAG:
            _gameScene->decreaseCarFactory();
            for (auto& building : *(_gameScene->getBuildings()))
            {
                if (building->getBuildingTag() == CAR_FACTORY_TAG)
                {
                    _gameScene->setCarFactoryPosition(building->getPosition());
                }
            }
            break;
        }
    }
}

void Manager::addMoneyUpdate()
{
    static clock_t preT = clock();
    clock_t nowT = clock();
    if (_gameScene->getMineNum() != 0)
    {
        if (nowT - preT >= addMoneyDelay)
        {
            _gameScene->addMoney(_gameScene->getMineNum() * 1000);
            preT = nowT;
        }
    }
}