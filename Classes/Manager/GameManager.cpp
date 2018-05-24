/*
*  @file     GameManager.cpp
*  @brief    游戏的控制类
*/

#include "GameManager.h"

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
    _selectedEnemy = _selectedBuilding = nullptr;
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
            if (!_gameScene->getHaveBarracks())
            {
                return;
            }
            castMoney = infantryCastMoney;
            castPower = infantryCastPower;
            break;
        case DOG_TAG:
            if (!_gameScene->getHaveBarracks())
            {
                return;
            }
            castMoney = dogCastMoney;
            castPower = dogCastPower;
            break;
        case TANK_TAG:
            if (!_gameScene->getHaveCarFactory())
            {
                return;
            }
            castMoney = tankCastMoney;
            castPower = tankCastPower;
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
        Building* building = Building::create(_buildingTag);
        building->setPosition(position);
        _gameScene->addChild(building);
        switch (_buildingTag)
        {
        case POWER_PLANT_TAG:
            _gameScene->setHavePowerPlant(true);
            _gameScene->addPower();                 // 加100电量                 
            break;
        case MINE_TAG:
            _gameScene->setHaveMine(true);
            break;
        case BARRACKS_TAG:
            _gameScene->setHaveBarracks(true);
            _gameScene->setBarracksPosition(true);  // 兵营位置 若造多个则只有最后一个有用
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
        Soldier* soldier = Soldier::create(_soldierTag);
        soldier->setPosition(_gameScene->getBarracksPosition());
        _gameScene->addChild(soldier);
        _gameScene->getSoldiers().pushBack(soldier);
        _canCreateSoldier = false;
        _waitToCreateSoldier = false;
    }
}

void Manager::setEnemy(Soldier* enemy)
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
    Vector<Soldier*>& enemySoldiers = _gameScene->getEnemySoldiers();
    clock_t nowT = clock();

    for (auto& soldier : _gameScene->getSoldiers())
    {
        if (_selectedEnemy && soldier->canAttack(_selectedEnemy->getPosition()))    // 如果选中了敌方士兵
        {
            switch (soldier->getTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getDelay())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getHP() <= 0)
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
                if (nowT - dogPreT >= soldier->getDelay())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getHP() <= 0)
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
                if (nowT - tankPreT >= soldier->getDelay())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getHP() <= 0)
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
                if (nowT - infantryPreT >= soldier->getDelay())
                {
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                    }
                    isInfantryAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getDelay())
                {
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                    }
                    isDogAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getDelay())
                {
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
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
                    if (nowT - infantryPreT >= soldier->getDelay())
                    {
                        soldier->attack(enemy);
                        if (enemy->getHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers.eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isInfantryAttack = true;
                    }
                    break;
                case DOG_TAG:
                    if (nowT - dogPreT >= soldier->getDelay())
                    {
                        soldier->attack(enemy);
                        if (enemy->getHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers.eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isDogAttack = true;
                    }
                    break;
                case TANK_TAG:
                    if (nowT - tankPreT >= soldier->getDelay())
                    {
                        soldier->attack(enemy);
                        if (enemy->getHP() <= 0)
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
}

