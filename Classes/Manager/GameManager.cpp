﻿/*
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
        int costMoney;
        switch (building_tag)
        {
        case POWER_PLANT_TAG:
            costMoney = buildingData::powerPlantCostMoney;       //  in BuildingData.h
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
            costMoney = buildingData::mineCostMoney;
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
            costMoney = buildingData::barracksCostMoney;
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
            costMoney = buildingData::carFactoryCostMoney;
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
        _gameScene->decreaseMoney(costMoney);
        
    }
}

void Manager::clickCreateSoldierByTag(Tag soldier_tag)
{
    int castMoney;
    switch (soldier_tag)
    {
    case INFANTRY_TAG:
        castMoney = unitData::infantryCostMoney;
        _gameScene->addInfantry();
        _soldierQueue.push(soldier_tag);
        break;

    case DOG_TAG:
        castMoney = unitData::dogCostMoney;
        _gameScene->addDog();
        _soldierQueue.push(soldier_tag);
        break;

    case TANK_TAG:
        castMoney = unitData::tankCostMoney;
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
	if (_panel == nullptr)
	{
		log("xtbl");
	}
	

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
            soldier->setGetDestination(false);
            //To Do:
            _gameScene->_client->sendMessage(MOVE_UNIT, _moveController->getMoveMessage(soldier, getPutSoldierPosition()));
            Vec2 direction = (soldier->getDestination() - soldier->getPosition());
            //change state of unit
            if (fabs(direction.x) < fabs(direction.y))
            {
                if (direction.y > 0)           //up
                {
                    soldier->switchState(stateWalkUp);
                }
                else                          //down
                {
                    soldier->switchState(stateWalkDown);
                }
            }
            else
            {
                //left
                if (direction.x < 0)
                {
                    soldier->switchState(stateWalkLeft);
                }
                //right
                else
                {
                    soldier->switchState(stateWalkRight);
                }
            }

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
    else if (_soldierQueue.size())
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
			_panel->_infantryIcon->showProgressOfWait(_waitTimeToCreateSoldier / 1000);
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
			_panel->_dogIcon->showProgressOfWait(_waitTimeToCreateSoldier / 1000);
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
            car->setGetDestination(false);
            //To Do:
            _gameScene->_client->sendMessage(MOVE_UNIT, _moveController->getMoveMessage(car, getPutCarPosition()));
            Vec2 direction = (car->getDestination() - car->getPosition());
            //change state of unit
            if (fabs(direction.x) < fabs(direction.y))
            {
                if (direction.y > 0)           //up
                {
                    car->switchState(stateWalkUp);
                }
                else                          //down
                {
                    car->switchState(stateWalkDown);
                }
            }
            else
            {
                //left
                if (direction.x < 0)
                {
                    car->switchState(stateWalkLeft);
                }
                //right
                else
                {
                    car->switchState(stateWalkRight);
                }
            }

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
			_panel->_tankIcon->showProgressOfWait(_waitTimeToCreateCar / 1000);
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
        int costPower;
        Building* building = Building::create(_buildingTag);
        _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
                (_gameScene->_gameListener->clone(), building);
        building->setPosition(position);
        //To Do:
        _gameScene->_client->sendMessage("createBuilding", "position&&tag");
        _gameScene->addChild(building, 2);
        switch (_buildingTag)
        {
        case POWER_PLANT_TAG:
            _gameScene->addPowerPlant();            // 电厂数量加一      
            _gameScene->addTotalPower(1000);
            _gameScene->addPower(1000);                 // 加100电量 
            costPower = buildingData::powerPlantCostPower;
            break;
        case MINE_TAG:
            _gameScene->addMine();                  // 矿场数量加一
            costPower = buildingData::mineCostPower;
            break;
        case BARRACKS_TAG:
            _gameScene->addBarracks();
            _gameScene->setBarracksPosition(building->getPosition());  // 兵营位置
            costPower = buildingData::barracksCostPower;
            break;
        case CAR_FACTORY_TAG:
            _gameScene->addCarFactory();
            _gameScene->setCarFactoryPosition(building->getPosition());
            costPower = buildingData::carFactoryCostPower;
            break;
        }
        _gameScene->getBuildings()->pushBack(building);
        _canCreateBuilding = false;
        _isWaitToCreateBuilding = false;
        _gameScene->decreasePower(costPower);
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
    bool isMySoldierDied = false;
    Unit* myDiedSoldier = nullptr;
    Tag selectedBuildingTag = NONE;
    Vector<Unit*>* enemySoldiers = _gameScene->getEnemySoldiers();
    clock_t nowT = clock();

    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        bool isSoldierAttack = false;

        if (_selectedEnemy && soldier->canAttack(_selectedEnemy->getPosition())
            && _selectedEnemy != soldier && !isSoldierAttack)    // 如果选中了敌方士兵  & cannot attak itself
        {
            //log("attack");
            switch (soldier->getUnitTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitATKCD())
                {
                    if (_selectedEnemy->getPositionX() - soldier->getPositionX() > 0)
                    {
                        soldier->switchState(stateAttackRight);
                    }
                    else
                    {
                        soldier->switchState(stateAttackLeft);
                    }
                    soldier->attack(_selectedEnemy);

                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->switchState(stateDeath);
                        if (_gameScene->getSoldiers()->contains(_selectedEnemy))
                        {
                            isMySoldierDied = true;
                            myDiedSoldier = _selectedEnemy;
                        }
                        else
                        {
                            enemySoldiers->eraseObject(_selectedEnemy);
                            _gameScene->removeChild(_selectedEnemy);
                        }
                        _selectedEnemy = nullptr;
                    }
                    isInfantryAttack = isSoldierAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitATKCD())
                {
                    if (_selectedEnemy->getPositionX() - soldier->getPositionX() > 0)
                    {
                        soldier->switchState(stateAttackRight);
                    }
                    else
                    {
                        soldier->switchState(stateAttackLeft);
                    }
                    soldier->attack(_selectedEnemy);

                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->switchState(stateDeath);
                        if (_gameScene->getSoldiers()->contains(_selectedEnemy))
                        {
                            isMySoldierDied = true;
                            myDiedSoldier = _selectedEnemy;
                        }
                        else
                        {
                            enemySoldiers->eraseObject(_selectedEnemy);
                            _gameScene->removeChild(_selectedEnemy);
                        }
                        _selectedEnemy = nullptr;
                    }
                    isDogAttack = isSoldierAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitATKCD())
                {
                    if (_selectedEnemy->getPositionX() - soldier->getPositionX() > 0)
                    {
                        soldier->switchState(stateAttackRight);
                    }
                    else
                    {
                        soldier->switchState(stateAttackLeft);
                    }
                    soldier->attack(_selectedEnemy);

                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->switchState(stateDeath);
                        if (_gameScene->getSoldiers()->contains(_selectedEnemy))
                        {
                            isMySoldierDied = true;
                            myDiedSoldier = _selectedEnemy;
                        }
                        else
                        {
                            enemySoldiers->eraseObject(_selectedEnemy);
                            _gameScene->removeChild(_selectedEnemy);
                        }
                        _selectedEnemy = nullptr;
                    }
                    isTankAttack = isSoldierAttack = true;
                }
                break;
            }

            continue;
        }
        else if (_selectedBuilding && soldier->canAttack(_selectedBuilding->getPosition()) && !isSoldierAttack)  // 如果选中了敌方建筑
        {
            selectedBuildingTag = _selectedBuilding->getBuildingTag();
            switch (soldier->getUnitTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitATKCD())
                {
                    isBuildingHurt = true;

                    if (_selectedBuilding->getPositionX() - soldier->getPositionX() > 0)
                    {
                        soldier->switchState(stateAttackRight);
                    }
                    else
                    {
                        soldier->switchState(stateAttackLeft);
                    }
                    soldier->attack(_selectedBuilding);

                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getEnemyBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                    }
                    isInfantryAttack = isSoldierAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitATKCD())
                {
                    isBuildingHurt = true;

                    if (_selectedBuilding->getPositionX() - soldier->getPositionX() > 0)
                    {
                        soldier->switchState(stateAttackRight);
                    }
                    else
                    {
                        soldier->switchState(stateAttackLeft);
                    }
                    soldier->attack(_selectedBuilding);

                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getEnemyBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        isBuildingDied = true;
                    }
                    isDogAttack = isSoldierAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitATKCD())
                {
                    isBuildingHurt = true;

                    if (_selectedBuilding->getPositionX() - soldier->getPositionX() > 0)
                    {
                        soldier->switchState(stateAttackRight);
                    }
                    else
                    {
                        soldier->switchState(stateAttackLeft);
                    }
                    soldier->attack(_selectedBuilding);

                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getEnemyBuildings()->eraseObject(_selectedBuilding);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        isBuildingDied = true;
                    }
                    isTankAttack = isSoldierAttack = true;
                }
            }

            continue;
        }

        for (auto& enemy : *(enemySoldiers))
        {
            if (isSoldierAttack)
            {
                break;
            }

            if (soldier->canAttack(enemy->getPosition()))
            {
                switch (soldier->getUnitTag())
                {
                case INFANTRY_TAG:
                    if (nowT - infantryPreT >= soldier->getUnitATKCD())
                    {
                        if (enemy->getPositionX() - soldier->getPositionX() > 0)
                        {
                            soldier->switchState(stateAttackRight);
                        }
                        else
                        {
                            soldier->switchState(stateAttackLeft);
                        }
                        soldier->attack(enemy);

                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->switchState(stateDeath);
                            enemySoldiers->eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isInfantryAttack = isSoldierAttack = true;
                    }
                    break;
                case DOG_TAG:
                    if (nowT - dogPreT >= soldier->getUnitATKCD())
                    {
                        if (enemy->getPositionX() - soldier->getPositionX() > 0)
                        {
                            soldier->switchState(stateAttackRight);
                        }
                        else
                        {
                            soldier->switchState(stateAttackLeft);
                        }
                        soldier->attack(enemy);

                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->switchState(stateDeath);
                            enemySoldiers->eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isDogAttack = isSoldierAttack = true;
                    }
                    break;
                case TANK_TAG:
                    if (nowT - tankPreT >= soldier->getUnitATKCD())
                    {
                        if (enemy->getPositionX() - soldier->getPositionX() > 0)
                        {
                            soldier->switchState(stateAttackRight);
                        }
                        else
                        {
                            soldier->switchState(stateAttackLeft);
                        }
                        soldier->attack(enemy);

                        if (enemy->getUnitHP() <= 0)
                        {
                            enemy->switchState(stateDeath);
                            enemySoldiers->eraseObject(enemy);
                            _gameScene->removeChild(enemy);
                        }
                        isTankAttack = isSoldierAttack = true;
                    }
                    break;
                }
                break;
            }
        }
    }
    if (isMySoldierDied)
    {
        _gameScene->getSoldiers()->eraseObject(myDiedSoldier);
        _gameScene->removeChild(myDiedSoldier);
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
            _gameScene->decreasePowerPlant();            // 电厂数量减一 
            this->resetPower();
            break;

        case MINE_TAG:
            _gameScene->decreaseMine();                  // 矿场数量-1
            _gameScene->addPower(buildingData::mineCostPower);
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
            _gameScene->addPower(buildingData::barracksCostPower);
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
            _gameScene->addPower(buildingData::carFactoryCostPower);
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
    if (_gameScene->getMineNum())
    {
        static clock_t preT = clock();
        clock_t nowT = clock();
        if (_gameScene->getMineNum() != 0)
        {
            if (nowT - preT >= addMoneyDelay)
            {
                _gameScene->addMoney(_gameScene->getMineNum() * 150);
                preT = nowT;
            }
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
            totalPower += (500 + building->getHP());
        }
        else
        {
            castPower += building->getCastPower();
        }
    }
    _gameScene->setTotalPower(totalPower);
    _gameScene->setPower(totalPower - castPower);
    // update power bar
    if (_gameScene->getPower() <= 0 || _gameScene->getTotalPower() == 0)
    {
        auto progressTo = ProgressTo::create(0.5f, 0);
        _gameScene->_powerBar->runAction(progressTo);
    }
    else
    {
        auto progressTo = ProgressTo::create(0.5f, _gameScene->getPower() * 100 
            / _gameScene->getTotalPower());
        _gameScene->_powerBar->runAction(progressTo);
    }
}

cocos2d::Point Manager::getPutSoldierPosition()
{
    Point barracksPosition = _gameScene->getBarracksPosition();
    Point firstPosition = barracksPosition - Vec2(80, 80);
    int soldierSize = 30;
    //log("getPutSoldier");
    //log("barracks %f %f", barracksPosition.x, barracksPosition.y);
    // 兵营下方区域
    for (int i = 4; i > 0; --i)
    {
        for (int j = 5; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(j*soldierSize, -i*soldierSize)))
            {
                //log("down %f %f", (firstPosition + Vec2(j*soldierSize, -i * soldierSize)).x,
                    //(firstPosition + Vec2(j*soldierSize, -i * soldierSize)).y);
                return firstPosition + Vec2(j*soldierSize, -i*soldierSize);
            }
        }
    }
    //兵营右方区域
    firstPosition += Vec2(160, 160);
    for (int i = 4; i > 0; --i)
    {
        for (int j = 4; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(j*soldierSize, i*soldierSize)))
            {
                return firstPosition + Vec2(j*soldierSize, i*soldierSize);
            }
        }
    }
    //那就别出来了
   return barracksPosition;
}

cocos2d::Point Manager::getPutCarPosition()
{
    Point carFactoryPosition = _gameScene->getCarFactoryPosition();
    Point firstPosition = carFactoryPosition - Vec2(100, 100);
    int carSize = 50;
    //车厂下方区域
    for (int i = 3; i > 0; --i)
    {
        for (int j = 5; j > 0; --j)
        {
            log("car down");
            if (_moveController->canPut(firstPosition + Vec2(j * carSize, -i*carSize)))
            {
                return firstPosition + Vec2(j * carSize, -i*carSize);
            }
        }
    }
    // 车厂右方区域
    firstPosition += Vec2(200, 200);
    for (int i = 4; i > 0; --i)
    {
        for (int j = 4; j > 0; --j)
        {
            if (_moveController->canPut(firstPosition + Vec2(j*carSize, i*carSize)))
            {
                return firstPosition + Vec2(j*carSize, i*carSize);
            }
        }
    }
    //那就别出来了
    return carFactoryPosition;
}

void Manager::setPanel(Panel* p)
{
	_panel = p;
}

void Manager::doCommands()
{
    std::string tempCommand;
    while ((tempCommand = _gameScene->_client->executeOrder()) != "no")
    {
        _gameScene->_commands.push(tempCommand);
    }

    while (_gameScene->_commands.size() != 0)
    {
        _command = _gameScene->_commands.front();
        _gameScene->_commands.pop();
        //To Do:
        bool set_enemy_position = true;
        if (set_enemy_position)
        {
			readMoveCommand();
            Vec2 destination = _destinationForMessage;
			int index = _index;
            Unit* enemy = _gameScene->getEnemySoldiers()->at(index);
            enemy->setDestination(destination);
            enemy->setGetDestination(false);
            Vec2 direction = (destination - enemy->getPosition());
            //change state of unit
            if (fabs(direction.x) < fabs(direction.y))
            {
                if (direction.y > 0)           //up
                {
                    enemy->switchState(stateWalkUp);
                }
                else                          //down
                {
                    enemy->switchState(stateWalkDown);
                }
            }
            else
            {
                //left
                if (direction.x < 0)
                {
                    enemy->switchState(stateWalkLeft);
                }
                //right
                else
                {
                    enemy->switchState(stateWalkRight);
                }
            }
        }
        //if (_command[0] /* == enemy move*/ )
        //{
        //    std::string name = std::string(&_command[1]);
        //    if (name == scene._localPlayerName)
        //    {
        //        //                std::cout << "LocalPlayerDead message" << std::endl;
        //        scene._player = scene._localPlayer;
        //        scene._player->stopAllActions();
        //        scene._localPlayer->setPlayerDead();

        //        executePlayer(delta, scene);
        //    }
        //    else {
        //        //                std::cout << "RemotePlayerDead message" << std::endl;
        //        scene._player = scene._remotePlayer[name];
        //        scene._player->stopAllActions();
        //        scene._player->setPlayerDead();
        //        executePlayer(delta, scene);
        //    }

        //}
    //    if (_command[0] /*!= KEY_ORDER[0]*/)
    //    {
    //        continue;
    //    }
    //    readCommand();
    //    // bool isPress;

    //    if (_playerName == scene._localPlayerName)
    //        continue;
    //    scene._player = scene._remotePlayer[_playerName];
    //    if (_code[0] == 'p')
    //    {
    //        scene._player->setKeys(scene._keyPressesedOrder[_code], true);
    //    }
    //    else
    //    {
    //        scene._player->setKeys(scene._keyReleasedOrder[_code], false);
    //    }

    //    executePlayer(delta, scene);
    //    scene._player->setPosition(_playerCurrentPosition);
    //}
    //for (int i = 0; i < scene._playerList.size(); i++)
    //{
    //    _playerName = scene._playerList.at(i).player_name;

    //    if (_playerName == scene._localPlayerName)
    //        continue;

    //    else {
    //        //std::cout << "PlayerName:" << _playerName << std::endl;
    //        scene._player = scene._remotePlayer[_playerName];
    //        executePlayer(delta, scene);
    //    }
    }
}

template <class Type>
Type stringToNum(const std::string& str) {
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

void Manager::readMoveCommand()
{
	auto leftBracket = _command.find('(');
	auto comma = _command.find(',');
	auto rightBracket = _command.find(')');

	std::string index(_command.begin() + 1, _command.begin() + 3);
	std::string playerId(_command.begin() + 3, _command.begin() + leftBracket);
	std::string spositionX(_command.begin() + 1 + leftBracket, _command.begin() + comma);
	std::string spositionY(_command.begin() + 1 + comma, _command.begin() + rightBracket);

	//std::cout << playerName << std::endl;

	float positionX = stringToNum<float>(spositionX);
	float positionY = stringToNum<float>(spositionY);
	//std::cout << positionX << std::endl;
	// std::cout << positionY << std::endl;
	_index = stringToNum<int>(index);
	_playerId = playerId;
	_destinationForMessage = Vec2(positionX, positionY);
}