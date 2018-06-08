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

    _isWaitToCreateBuilding = false;
    _isWaitToCreateSoldier = false;
    _isWaitToCreateCar = false;
    _canCreateBuilding = false;
    _selectedEnemy = nullptr;
    _selectedBuilding = nullptr;

    return true;
}

void Manager::clickCreateBuildingByTag(Tag building_tag, clock_t start_time)
{
    if (!_isWaitToCreateBuilding)
    {
        int castMoney, castPower;
        switch (building_tag)
        {
        case POWER_PLANT_TAG:
            castMoney = buildingData::powerPlantCastMoney;       //  in BuildingData.h
            castPower = buildingData::powerPlantCastPower;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateBuilding = buildingData::EnoughPower::powerPlantWait;
            }
            else
            {
                _waitTimeToCreateBuilding = buildingData::NotEnoughPower::powerPlantWait;
            }
            break;
        case MINE_TAG:
            castMoney = buildingData::mineCastMoney;
            castPower = buildingData::mineCastPower;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateBuilding = buildingData::EnoughPower::mineWait;
            }
            else
            {
                _waitTimeToCreateBuilding = buildingData::NotEnoughPower::mineWait;
            }
            break;
        case BARRACKS_TAG:
            castMoney = buildingData::barracksCastMoney;
            castPower = buildingData::barracksCastPower;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateBuilding = buildingData::EnoughPower::barracksWait;
            }
            else
            {
                _waitTimeToCreateBuilding = buildingData::NotEnoughPower::barracksWait;
            }
            break;
        case CAR_FACTORY_TAG:
            castMoney = buildingData::carFactoryCastMoney;
            castPower = buildingData::carFactoryCastPower;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateBuilding = buildingData::EnoughPower::carFactoryWait;
            }
            else
            {
                _waitTimeToCreateBuilding = buildingData::NotEnoughPower::carFactoryWait;
            }
            break;
        }

        // 图标处会判断钱是否足够
        _isWaitToCreateBuilding = true;
        _timeToCreateBuilding = start_time;
        _buildingTag = building_tag;
        _gameScene->decreaseMoney(castMoney);
        _gameScene->decreasePower(castPower);
        
    }
}

void Manager::clickCreateSoldierByTag(Tag soldier_tag)
{
    int castMoney;
    switch (soldier_tag)
    {
    case INFANTRY_TAG:
        castMoney = unitData::infantryCastMoney;
        _gameScene->addInfantry();
        _soldierQueue.push(soldier_tag);
        break;

    case DOG_TAG:
        castMoney = unitData::dogCastMoney;
        _gameScene->addDog();
        _soldierQueue.push(soldier_tag);
        break;

    case TANK_TAG:
        castMoney = unitData::tankCastMoney;
        _gameScene->addTank();
        _carQueue.push(soldier_tag);
        break;
    }

    _gameScene->decreaseMoney(castMoney);

}

void Manager::waitCreateBuilding()
{
    if (_isWaitToCreateBuilding)
    {
        if (clock() - _timeToCreateBuilding > _waitTimeToCreateBuilding)
        {
            _canCreateBuilding = true;
        }
    }
}

void Manager::waitCreateSoldier()
{
    if (_isWaitToCreateSoldier)
    {
        if (clock() - _timeToCreateSoldier > _waitTimeToCreateSoldier)
        {
            Unit* soldier = Unit::create(_soldierTag);
            _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
                    (_gameScene->_gameListener->clone(), soldier);
            
			soldier->setPosition(_gameScene->getBarracksPosition());
            _gameScene->addChild(soldier, 1);
            // 走出兵营
            soldier->setDestination(getPutSoldierPosition());

            _gameScene->getSoldiers()->pushBack(soldier);
            _soldierQueue.pop();
            _isWaitToCreateSoldier = false;

            switch (_soldierTag)
            {
            case INFANTRY_TAG:
                _gameScene->decreaseInfantry();
                break;

            case DOG_TAG:
                _gameScene->decreaseDog();
                break;
            }
        }
    }
    else if (_soldierQueue.size() > 0)
    {
        switch (_soldierQueue.front())
        {
        case INFANTRY_TAG:
            if (_gameScene->getBarracksNum() == 0)
            {
                return;
            }
            _soldierTag = INFANTRY_TAG;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateSoldier = unitData::EnoughPower::infantryWait;
            }
            else
            {
                _waitTimeToCreateSoldier = unitData::NotEnoughPower::infantryWait;
            }
			_gameScene->panel->_infantryIcon->showProgressOfWait(_waitTimeToCreateSoldier/1000);
            break;

        case DOG_TAG:
            if (_gameScene->getBarracksNum() == 0)
            {
                return;
            }
            _soldierTag = DOG_TAG;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateSoldier = unitData::EnoughPower::dogWait;
            }
            else
            {
                _waitTimeToCreateSoldier = unitData::NotEnoughPower::dogWait;
            }
			_gameScene->panel->_dogIcon->showProgressOfWait(_waitTimeToCreateSoldier/1000);
            break;

        case TANK_TAG:
            if (_gameScene->getCarFactoryNum() == 0)
            {
                return;
            }
            _carTag = TANK_TAG;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateCar = unitData::EnoughPower::tankWait;
            }
            else
            {
                _waitTimeToCreateCar = unitData::NotEnoughPower::tankWait;
            }
			_gameScene->panel->_tankIcon->showProgressOfWait(_waitTimeToCreateCar / 1000);
            break;
        }

        _isWaitToCreateSoldier = true;
        _timeToCreateSoldier = clock();

    }
}

void Manager::waitCreateCar()
{
    if (_isWaitToCreateCar)
    {
        if (clock() - _timeToCreateCar > _waitTimeToCreateCar)
        {
            Unit* car = Unit::create(_carTag);
            _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
            (_gameScene->_gameListener->clone(), car);

            car->setPosition(_gameScene->getCarFactoryPosition());
            _gameScene->addChild(car, 1);
            // 开出车厂
            car->setDestination(getPutCarPosition());

            _gameScene->getSoldiers()->pushBack(car);
            _carQueue.pop();
            _isWaitToCreateCar = false;

            switch (_carTag)
            {
            case TANK_TAG:
                _gameScene->decreaseTank();
                break;
            }
        }
    }
    else if (_carQueue.size() > 0)
    {
        switch (_carQueue.front())
        {
        case TANK_TAG:
            if (_gameScene->getCarFactoryNum() == 0)
            {
                return;
            }
            _carTag = TANK_TAG;
            if (_gameScene->getIsPowerEnough())
            {
                _waitTimeToCreateCar = unitData::EnoughPower::tankWait;
            }
            else
            {
                _waitTimeToCreateCar = unitData::NotEnoughPower::tankWait;
            }
            break;
        }

        _isWaitToCreateCar = true;
        _timeToCreateCar = clock();

    }
}

void Manager::createBuilding(cocos2d::Vec2 position)
{
    if (_canCreateBuilding)
    {
        Building* building = Building::create(_buildingTag);
        _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
                (_gameScene->_gameListener->clone(), building);
        building->setPosition(position);
        _gameScene->addChild(building, 2);
        switch (_buildingTag)
        {
        case POWER_PLANT_TAG:
            _gameScene->addPowerPlant();            // 电厂数量加一
            _gameScene->addPower(100);                 // 加100电量       
            _gameScene->addTotalPower(100);
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
        _isWaitToCreateBuilding = false;
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
    bool isBuildingHurt = false;
    Tag selectedBuildingTag = NONE;
    Vector<Unit*>* enemySoldiers/* = _gameScene->getEnemySoldiers()*/;
    clock_t nowT = clock();

    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (_selectedEnemy && soldier->canAttack(_selectedEnemy->getPosition()))    // 如果选中了敌方士兵
        {
            switch (soldier->getUnitTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitATKCD())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->setDeath();
                        enemySoldiers->eraseObject(_selectedEnemy);
                        _gameScene->removeChild(_selectedEnemy);
                        _selectedEnemy = nullptr;
                    }
                    isInfantryAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitATKCD())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->setDeath();
                        enemySoldiers->eraseObject(_selectedEnemy);
                        _gameScene->removeChild(_selectedEnemy);
                        _selectedEnemy = nullptr;
                    }
                    isDogAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitATKCD())
                {
                    soldier->attack(_selectedEnemy);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->setDeath();
                        enemySoldiers->eraseObject(_selectedEnemy);
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
            selectedBuildingTag = _selectedBuilding->getBuildingTag();
            switch (soldier->getUnitTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitATKCD())
                {
                    isBuildingHurt = true;
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                    }
                    isInfantryAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitATKCD())
                {
                    isBuildingHurt = true;
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        isBuildingDied = true;
                    }
                    isDogAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitATKCD())
                {
                    isBuildingHurt = true;
                    soldier->attack(_selectedBuilding);
                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
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

        for (auto& enemy : *(enemySoldiers))
        {
            if (soldier->canAttack(enemy->getPosition()))
            {
                switch (soldier->getUnitTag())
                {
                case INFANTRY_TAG:
                    if (nowT - infantryPreT >= soldier->getUnitATKCD())
                    {
                        soldier->attack(enemy);
                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers->eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isInfantryAttack = true;
                    }
                    break;
                case DOG_TAG:
                    if (nowT - dogPreT >= soldier->getUnitATKCD())
                    {
                        soldier->attack(enemy);
                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers->eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isDogAttack = true;
                    }
                    break;
                case TANK_TAG:
                    if (nowT - tankPreT >= soldier->getUnitATKCD())
                    {
                        soldier->attack(enemy);
                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->setDeath();
                            enemySoldiers->eraseObject(enemy);
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
            this->resetPower();
            break;

        case MINE_TAG:
            _gameScene->decreaseMine();                  // 矿场数量-1
            _gameScene->addPower(buildingData::mineCastPower);
            break;

        case BARRACKS_TAG:
            _gameScene->decreaseBarracks();
            if (_gameScene->getBarracksNum())
            {
                for (auto& building : *(_gameScene->getBuildings()))
                {
                    if (building->getBuildingTag() == BARRACKS_TAG)
                    {
                        _gameScene->setBarracksPosition(building->getPosition());
                    }
                }
            }
            else
            {
                _isWaitToCreateSoldier = false;
            }
            _gameScene->addPower(buildingData::barracksCastPower);
            break;

        case CAR_FACTORY_TAG:
            _gameScene->decreaseCarFactory();
            if (_gameScene->getCarFactoryNum())
            {
                for (auto& building : *(_gameScene->getBuildings()))
                {
                    if (building->getBuildingTag() == CAR_FACTORY_TAG)
                    {
                        _gameScene->setCarFactoryPosition(building->getPosition());
                    }
                }
            }
            else
            {
                _isWaitToCreateCar = false;
            }
            _gameScene->addPower(buildingData::carFactoryCastPower);
            break;
        }
    }
    else if (isBuildingHurt)
    {
        switch (selectedBuildingTag)
        {
        case POWER_PLANT_TAG:          
            this->resetPower();
            break;
        default:
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

void Manager::resetPower()
{
    int castPower = 0;
    int totalPower = 0;
    for (auto& building : *(_gameScene->getBuildings()))
    {
        if (building->getBuildingTag() == POWER_PLANT_TAG)
        {
            totalPower += (50 + 5 * building->getHP() / 10);
        }
        else
        {
            castPower += building->getCastPower();
        }
    }
    _gameScene->setTotalPower(totalPower);
    _gameScene->setPower(totalPower - castPower);
}

cocos2d::Point Manager::getPutSoldierPosition()
{
    Point barracksPosition = _gameScene->getBarracksPosition();
    Point firstPosition = barracksPosition - Vec2(101, 101);
    int soldierSize = 40;
    // 兵营下方区域
    for (int i = 4; i > 0; --i)
    {
        for (int j = 5; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(j*soldierSize, -i*soldierSize)))
            {
                return firstPosition + Vec2(j*soldierSize, -i*soldierSize);
            }
        }
    }
    //兵营左方区域
    for (int i = 4; i > 0; --i)
    {
        for (int j = 4; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(-j * soldierSize, i*soldierSize)))
            {
                return firstPosition + Vec2(-j * soldierSize, i*soldierSize);
            }
        }
    }
    //那就别出来了
   return barracksPosition;
}

cocos2d::Point Manager::getPutCarPosition()
{
    Point carFactoryPosition = _gameScene->getCarFactoryPosition();
    Point firstPosition = carFactoryPosition + Vec2(270, 220);
    int carSize = 80;
    // 车厂右方区域
    for (int i = 5; i > 0; --i)
    {
        for (int j = 3; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(j*carSize, i*carSize)))
            {
                return firstPosition + Vec2(j*carSize, i*carSize);
            }
        }
    }
    //车厂上方区域
    for (int i = 5; i > 3; --i)
    {
        for (int j = 2; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(-j * carSize, i*carSize)))
            {
                return firstPosition + Vec2(-j * carSize, i*carSize);
            }
        }
    }
    //那就别出来了
    return carFactoryPosition;
}
