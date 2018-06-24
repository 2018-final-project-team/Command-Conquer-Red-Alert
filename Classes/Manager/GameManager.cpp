/*
*  @file     GameManager.cpp
*  @brief    游戏的控制类
*/

#include "GameManager.h"
#include "cocos2d.h"
#include <cstdlib>

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
            Unit* soldier = Unit::create(_soldierTag, _gameScene->_localPlayerID);
            _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
                    (_gameScene->_gameListener->clone(), soldier);
			_gameScene->_client->sendMessage(CREATE_UNIT, getCreateUnitMessage(_soldierTag, _gameScene->getBarracksPosition()));
			soldier->setPosition(_gameScene->getBarracksPosition());
			_gameScene->getSoldiers()->pushBack(soldier);
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
            Unit* car = Unit::create(_carTag, _gameScene->_localPlayerID);
            _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
            (_gameScene->_gameListener->clone(), car);
			_gameScene->_client->sendMessage(CREATE_UNIT, getCreateUnitMessage(_carTag, _gameScene->getCarFactoryPosition()));

            car->setPosition(_gameScene->getCarFactoryPosition());
			_gameScene->getSoldiers()->pushBack(car);

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
        Building* building = Building::create(_buildingTag, _gameScene->_localPlayerID);
        _gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
                (_gameScene->_gameListener->clone(), building);
        building->setPosition(position);
        //To Do:
        _gameScene->_client->sendMessage(CREATE_BUILDING, getCreateBuildingMessage(position, _buildingTag));
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
    //移动攻击.
    if (_gameScene->getSelectedSoldiers()->size() > 0)
    {
        Vec2 direction = _gameScene->getSelectedSoldiers()->front()->getPosition()
            - enemy->getPosition();
        direction.normalize();
        _moveController->setDestination(enemy->getPosition() + direction * 30);
    }

    if (_gameScene->getSoldiers()->contains(_selectedEnemy))
    {
        _selectedEnemyId = _gameScene->_localPlayerID;
        _selectedEnemyIndex = _gameScene->getSoldiers()->getIndex(_selectedEnemy);
    }
    else
    {
        _selectedEnemyId = _selectedEnemy->getID();
        _selectedEnemyIndex = _gameScene->getEnemySoldiersByID(_selectedEnemyId)->getIndex(_selectedEnemy);
    }
}

void Manager::setBuilding(Building* building)
{
    _selectedBuilding = building;
    _selectedEnemy = nullptr;
    //移动攻击.
    if (_gameScene->getSelectedSoldiers()->size() == 0)
    {
        return;
    }
    Vec2 direction = _gameScene->getSelectedSoldiers()->front()->getPosition()
        - building->getPosition();
    direction.normalize();
    _moveController->setDestination(building->getPosition() + direction * 80);
    
    _selectedBuildingId = _selectedBuilding->getID();
    _selectedBuildingIndex = _gameScene->
        getEnemyBuildingsByID(_selectedBuildingId)->getIndex(_selectedBuilding);
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
    clock_t nowT = clock();

    for (int i = 0; i < _gameScene->getSoldiers()->size(); ++i)
    {
        auto soldier = _gameScene->getSoldiers()->at(i);
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
                    _gameScene->_client->sendMessage(ATTACK_UNIT, 
                        getAttackMessage(soldier, _selectedEnemyId, _selectedEnemyIndex));
                    log("attack enemy id %d index %d", _selectedEnemyId, _selectedEnemyIndex);
                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->switchState(stateDeath);
                        if (_gameScene->_localPlayerID == _selectedEnemyId)
                        {
                            if (_gameScene->getSoldiers()->getIndex(_selectedEnemy) < i)
                            {
                                --i;
                            }
                            _gameScene->getSoldiers()->erase(_selectedEnemyIndex);
                        }
                        else
                        {
                            _gameScene->getEnemySoldiersByID(_selectedEnemyId)->erase(_selectedEnemyIndex);
                        }
                        _gameScene->removeChild(_selectedEnemy);
                        _gameScene->_client->sendMessage(UNIT_DIED, getDeathMessage(_gameScene->_localPlayerID,
                            _selectedEnemyId, _selectedEnemyIndex));
                        _selectedEnemy = nullptr;
                        return;
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
                    _gameScene->_client->sendMessage(ATTACK_UNIT, 
                        getAttackMessage(soldier, _selectedEnemyId, _selectedEnemyIndex));

                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->switchState(stateDeath);
                        if (_gameScene->_localPlayerID == _selectedEnemyId)
                        {
                            if (_gameScene->getSoldiers()->getIndex(_selectedEnemy) < i)
                            {
                                --i;
                            }
                            _gameScene->getSoldiers()->erase(_selectedEnemyIndex);
                        }
                        else
                        {
                            _gameScene->getEnemySoldiersByID(_selectedEnemyId)->erase(_selectedEnemyIndex);
                        }
                        _gameScene->removeChild(_selectedEnemy);
                        _gameScene->_client->sendMessage(UNIT_DIED, getDeathMessage(_gameScene->_localPlayerID,
                            _selectedEnemyId, _selectedEnemyIndex));
                        _selectedEnemy = nullptr;
                        return;
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
                    _gameScene->_client->sendMessage(ATTACK_UNIT, 
                        getAttackMessage(soldier, _selectedEnemyId, _selectedEnemyIndex));

                    if (_selectedEnemy->getUnitHP() <= 0)
                    {
                        _selectedEnemy->switchState(stateDeath);
                        if (_gameScene->_localPlayerID == _selectedEnemyId)
                        {
                            if (_gameScene->getSoldiers()->getIndex(_selectedEnemy) < i)
                            {
                                --i;
                            }
                            _gameScene->getSoldiers()->erase(_selectedEnemyIndex);
                        }
                        else
                        {
                            _gameScene->getEnemySoldiersByID(_selectedEnemyId)->erase(_selectedEnemyIndex);
                        }
                        _gameScene->removeChild(_selectedEnemy);
                        _gameScene->_client->sendMessage(UNIT_DIED, getDeathMessage(_gameScene->_localPlayerID,
                            _selectedEnemyId, _selectedEnemyIndex));
                        _selectedEnemy = nullptr;
                        return;
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
                    _gameScene->_client->sendMessage(ATTACK_BUILDING,
                        getAttackMessage(soldier, _selectedBuildingId, _selectedBuildingIndex));

                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getEnemyBuildingsByID(_selectedBuildingId)->erase(_selectedBuildingIndex);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        _gameScene->_client->sendMessage(BUILDING_DIED, getDeathMessage(_gameScene->_localPlayerID,
                            _selectedBuildingId, _selectedBuildingIndex));
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
                    _gameScene->_client->sendMessage(ATTACK_BUILDING,
                        getAttackMessage(soldier, _selectedBuildingId, _selectedBuildingIndex));

                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getEnemyBuildingsByID(_selectedBuildingId)->erase(_selectedBuildingIndex);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        _gameScene->_client->sendMessage(BUILDING_DIED, getDeathMessage(_gameScene->_localPlayerID,
                            _selectedBuildingId, _selectedBuildingIndex));
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
                    _gameScene->_client->sendMessage(ATTACK_BUILDING,
                        getAttackMessage(soldier, _selectedBuildingId, _selectedBuildingIndex));

                    if (_selectedBuilding->getHP() <= 0)
                    {
                        _selectedBuilding->setDeath();
                        isBuildingDied = true;
                        _gameScene->getEnemyBuildingsByID(_selectedBuildingId)->erase(_selectedBuildingIndex);
                        _gameScene->removeChild(_selectedBuilding);
                        _selectedBuilding = nullptr;
                        _gameScene->_client->sendMessage(BUILDING_DIED, getDeathMessage(_gameScene->_localPlayerID,
                            _selectedBuildingId, _selectedBuildingIndex));
                    }
                    isTankAttack = isSoldierAttack = true;
                }
            }

            continue;
        }

        for (int id = 1; id <= 4; ++id)
        {
            for (int i = 0; i < _gameScene->getEnemySoldiersByID(id)->size(); ++i)
            {
                auto enemy = _gameScene->getEnemySoldiersByID(id)->at(i);
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
                            _gameScene->_client->sendMessage(ATTACK_UNIT,
                                getAttackMessage(soldier, id, i));

                            if (enemy->getUnitHP() <= 0)
                            {
                                enemy->switchState(stateDeath);
                                _gameScene->getEnemySoldiersByID(id)->erase(i);
                                _gameScene->removeChild(enemy);
                                _gameScene->_client->sendMessage(UNIT_DIED,
                                    getDeathMessage(_gameScene->_localPlayerID, id, i));
                                return;
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
                            //To Do id
                            _gameScene->_client->sendMessage(ATTACK_UNIT,
                                getAttackMessage(soldier, id, i));

                            if (enemy->getUnitHP() <= 0)
                            {
                                enemy->switchState(stateDeath);
                                _gameScene->getEnemySoldiersByID(id)->erase(i);
                                _gameScene->removeChild(enemy);
                                _gameScene->_client->sendMessage(UNIT_DIED,
                                    getDeathMessage(_gameScene->_localPlayerID, id, i));
                                return;
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
                            //To Do id
                            _gameScene->_client->sendMessage(ATTACK_UNIT,
                                getAttackMessage(soldier, id, i));

                            if (enemy->getUnitHP() <= 0)
                            {
                                enemy->switchState(stateDeath);
                                _gameScene->getEnemySoldiersByID(id)->erase(i);
                                _gameScene->removeChild(enemy);
                                _gameScene->_client->sendMessage(UNIT_DIED,
                                    getDeathMessage(_gameScene->_localPlayerID, id, i));
                                return;
                            }
                            isTankAttack = isSoldierAttack = true;
                        }
                        break;
                    }
                    break;
                }
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

void Manager::buildingDied(Tag buildingTag)
{
    switch (buildingTag)
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
    int isUnitDied[5][100];
    std::memset(isUnitDied, 0, sizeof(isUnitDied));
    int isBuildingDied[5][100];
    std::memset(isBuildingDied, 0, sizeof(isBuildingDied));
    while (_gameScene->_commands.size() != 0)
    {
        _command = _gameScene->_commands.front();
        _gameScene->_commands.pop();

        if (_command[0] == MOVE_UNIT[0])
        {
			readMoveCommand();
			if (_playerId == _gameScene->_localPlayerID)
			{
				continue;
			}
            Vec2 destination = _destinationForMessage;
			int index = _index;
            Unit* enemy = _gameScene->getEnemySoldiersByID(_playerId)->at(index);
            //log("command enemy position %f %f", enemy->getPosition().x, enemy->getPosition().y);
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
		else if (_command[0] == CREATE_BUILDING[0])
		{
			readCreateBuildingCommand();
			if (_playerId == _gameScene->_localPlayerID)
			{
				continue;
			}
			Vec2 position = _positionForMessage;
			Tag buildingTag = _tagForMessage;
			Building* building = Building::create(buildingTag, _playerId);
			_gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
			(_gameScene->_gameListener->clone(), building);
			building->setPosition(position);
			building->_bloodBarPt->setVisible(false);
			building->_bloodBarAsEnemyPt->setVisible(true);
			_gameScene->addChild(building, 2);
			_gameScene->pushEnemyBuildingByID(building, _playerId);

		}
		else if (_command[0] == CREATE_UNIT[0])
		{
			readCreateUnitCommand();
			if (_playerId == _gameScene->_localPlayerID)
			{
				continue;
			}
			Unit* soldier = Unit::create(_tagForMessage, _playerId);
			_gameScene->_gameEventDispatcher->addEventListenerWithSceneGraphPriority
			(_gameScene->_gameListener->clone(), soldier);

			soldier->setPosition(_positionForMessage);
			soldier->setDestination(_positionForMessage);
			soldier->_bloodBarPt->setVisible(false);
			soldier->_bloodBarAsEnemyPt->setVisible(true);
			_gameScene->addChild(soldier, 1);

            _gameScene->pushEnemyUnitByID(soldier, _playerId);
		}
		else if (_command[0] == REMOVE_BUILDING[0])
		{
			readRemoveBuildingCommand();
			if (_playerId == _gameScene->_localPlayerID)
			{
				continue;
			}
			_gameScene->removeChild(_gameScene->getEnemyBuildingsByID(_playerId)->at(_index));
			_gameScene->getEnemyBuildingsByID(_playerId)->erase(_index);
		}
		else if (_command[0] == REMOVE_UNIT[0])
		{
			readRemoveUnitCommand();
			if (_playerId == _gameScene->_localPlayerID)
			{
				continue;
			}
			_gameScene->removeChild(_gameScene->getEnemySoldiersByID(_playerId)->at(_index));
			_gameScene->getEnemySoldiersByID(_playerId)->erase(_index);
		}
        else if (_command[0] == ATTACK_UNIT[0])
        {
            readAttackCommand();
            if (_playerId == _gameScene->_localPlayerID || isUnitDied[_enemyId][_enemyIndex]
                || isUnitDied[_playerId][_index])
            {
                continue;
            }
            Unit* soldier;
            Unit* enemy;
            //=================get enemy through index==================
            if (_enemyId == _gameScene->_localPlayerID)
            {
                enemy = _gameScene->getSoldiers()->at(_enemyIndex);
            }
            else
            {
                enemy = _gameScene->getEnemySoldiersByID(_enemyId)->at(_enemyIndex);
            }
            cocos2d::log("get enemy id %d index %d", _enemyId, _enemyIndex);
            //====================get attacker through index========================
            cocos2d::log("get attacker playerId %d index %d", _playerId, _index);
            soldier = _gameScene->getEnemySoldiersByID(_playerId)->at(_index);

            //=========================attack============================
            if (enemy->getPositionX() - soldier->getPositionX() > 0)
            {
                soldier->switchState(stateAttackRight);
            }
            else
            {
                soldier->switchState(stateAttackLeft);
            }
            soldier->attack(enemy);
        }
        else if (_command[0] == ATTACK_BUILDING[0])
        {
            readAttackCommand();
            if (_playerId == _gameScene->_localPlayerID || isBuildingDied[_enemyId][_enemyIndex]
                || isUnitDied[_playerId][_index])
            {
                continue;
            }
            Unit* soldier;
            Building* enemy;
            bool powerHurt = false;
            //=================get enemy through index==================
            if (_enemyId == _gameScene->_localPlayerID)
            {
                enemy = _gameScene->getBuildings()->at(_enemyIndex);
                if (enemy->getBuildingTag() == POWER_PLANT_TAG)
                {
                    powerHurt = true;
                }
            }
            else
            {
                enemy = _gameScene->getEnemyBuildingsByID(_enemyId)->at(_enemyIndex);
            }

            //====================get attacker through index========================
            soldier = _gameScene->getEnemySoldiersByID(_playerId)->at(_index);
                
            //=========================attack============================
            if (enemy->getPositionX() - soldier->getPositionX() > 0)
            {
                soldier->switchState(stateAttackRight);
            }
            else
            {
                soldier->switchState(stateAttackLeft);
            }
            soldier->attack(enemy);
            if (powerHurt)
            {
                resetPower();
            }
        }
        else if (_command[0] == UNIT_DIED[0])
        {
            readDeathCommand();
            if (_playerId == _gameScene->_localPlayerID || isUnitDied[_enemyId][_enemyIndex])
            {
                if (_playerId == _gameScene->_localPlayerID)
                {
                    isUnitDied[_enemyId][_enemyIndex] = 1;
                }
                continue;
            }
            if (_enemyId == _gameScene->_localPlayerID)
            {
                _gameScene->removeChild(_gameScene->getSoldiers()->at(_enemyIndex));
                _gameScene->getSoldiers()->erase(_enemyIndex);
            }
            else
            {
                _gameScene->removeChild(_gameScene->getEnemySoldiersByID(_enemyId)->at(_enemyIndex));
                _gameScene->getEnemySoldiersByID(_enemyId)->erase(_enemyIndex);
            }
            isUnitDied[_enemyId][_enemyIndex] = 1;
            log("unit die enemyid %d enemyindex %d", _enemyId, _enemyIndex);
        }
        else if (_command[0] == BUILDING_DIED[0])
        {
            readDeathCommand();
            if (_playerId == _gameScene->_localPlayerID || isBuildingDied[_enemyId][_enemyIndex])
            {
                continue;
            }
            if (_enemyId == _gameScene->_localPlayerID)
            {
                _gameScene->removeChild(_gameScene->getBuildings()->at(_enemyIndex));
                _gameScene->getBuildings()->erase(_enemyIndex);
                buildingDied(_gameScene->getBuildings()->at(_enemyIndex)->getBuildingTag());
            }
            else
            {
                _gameScene->removeChild(_gameScene->getEnemyBuildingsByID(_enemyId)->at(_enemyIndex));
                _gameScene->getEnemyBuildingsByID(_enemyId)->erase(_enemyIndex);
            }
            isBuildingDied[_enemyId][_enemyIndex] = 1;
        }
		else
		{
			continue;
		}
       
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
	_playerId = stringToNum<int>(playerId);
	_destinationForMessage = Vec2(positionX, positionY);
	_destinationForMessage = _gameScene->_tileMap->convertToWorldSpace(_destinationForMessage);
}


std::string Manager::getCreateBuildingMessage(cocos2d::Vec2 pos, Tag tag)
{
	//格式：玩家id + Tag + (X,Y)
	std::stringstream ssPlayerId;
	std::stringstream ssTag;
	std::stringstream ssX;
	std::stringstream ssY;

	std::string s1 = "(";
	std::string s2 = ",";
	std::string s3 = ")";

	ssPlayerId.fill(0);
	ssPlayerId.width(2);
	ssPlayerId << _gameScene->_localPlayerID;
	std::string sId = ssPlayerId.str();
	if (sId[0] == '\0')
	{
		sId[0] = '0';
	}

	ssTag.fill(0);
	ssTag.width(2);
	ssTag << static_cast<int>(tag);
	std::string sTag = ssTag.str();
	if (sTag[0] == '\0')
	{
		sTag[0] = '0';
	}

	pos = _gameScene->_tileMap->convertToNodeSpace(pos);

	ssX << pos.x;
	std::string sX = ssX.str();

	ssY << pos.y;
	std::string sY = ssY.str();

	return sId + sTag + s1 + sX + s2 + sY + s3;
}

void Manager::readCreateBuildingCommand()
{
	auto leftBracket = _command.find('(');
	auto comma = _command.find(',');
	auto rightBracket = _command.find(')');

	std::string sPlayerId(_command.begin() + 1, _command.begin() + 3);
	std::string sTag(_command.begin() + 3, _command.begin() + leftBracket);
	std::string spositionX(_command.begin() + 1 + leftBracket, _command.begin() + comma);
	std::string spositionY(_command.begin() + 1 + comma, _command.begin() + rightBracket);

	//std::cout << playerName << std::endl;

	float positionX = stringToNum<float>(spositionX);
	float positionY = stringToNum<float>(spositionY);
	//std::cout << positionX << std::endl;
	// std::cout << positionY << std::endl;
	
	_playerId = stringToNum<int>(sPlayerId);
	int numTag = stringToNum<int>(sTag);
	_tagForMessage = static_cast<Tag>(numTag);
	_positionForMessage = Vec2(positionX, positionY);
	_positionForMessage = _gameScene->_tileMap->convertToWorldSpace(_positionForMessage);
}


std::string Manager::getCreateUnitMessage(Tag tag, Vec2 pos)
{
	//格式：玩家id + Tag + 出生点位置（兵营/车厂）
	std::stringstream ssPlayerId;
	std::stringstream ssTag;
	std::stringstream ssX;
	std::stringstream ssY;

	std::string s1 = "(";
	std::string s2 = ",";
	std::string s3 = ")";

	ssPlayerId.fill(0);
	ssPlayerId.width(2);
	ssPlayerId << _gameScene->_localPlayerID;
	std::string sId = ssPlayerId.str();
	if (sId[0] == '\0')
	{
		sId[0] = '0';
	}

	ssTag.fill(0);
	ssTag.width(2);
	ssTag << static_cast<int>(tag);
	std::string sTag = ssTag.str();
	if (sTag[0] == '\0')
	{
		sTag[0] = '0';
	}

	pos = _gameScene->_tileMap->convertToNodeSpace(pos);

	ssX << pos.x;
	std::string sX = ssX.str();

	ssY << pos.y;
	std::string sY = ssY.str();

	return sId + sTag + s1 + sX + s2 + sY + s3;
}


void Manager::readCreateUnitCommand()
{
	auto leftBracket = _command.find('(');
	auto comma = _command.find(',');
	auto rightBracket = _command.find(')');

	std::string sPlayerId(_command.begin() + 1, _command.begin() + 3);
	std::string sTag(_command.begin() + 3, _command.begin() + 5);
	std::string spositionX(_command.begin() + 1 + leftBracket, _command.begin() + comma);
	std::string spositionY(_command.begin() + 1 + comma, _command.begin() + rightBracket);

	float positionX = stringToNum<float>(spositionX);
	float positionY = stringToNum<float>(spositionY);

	_playerId = stringToNum<int>(sPlayerId);
	int numTag = stringToNum<int>(sTag);
	_tagForMessage = static_cast<Tag>(numTag);
	_positionForMessage = Vec2(positionX, positionY);
	_positionForMessage = _gameScene->_tileMap->convertToWorldSpace(_positionForMessage);
}

std::string Manager::getRemoveBuildingMessage(Building* b)
{
	//格式：索引 + 玩家id
	auto index = _gameScene->getBuildings()->getIndex(b);

	std::stringstream ssIndex;
	std::stringstream ssPlayerId;

	ssIndex.fill(0);            //位宽为2，左侧补零
	ssIndex.width(2);
	ssIndex << index;
	std::string sIndex = ssIndex.str();
	if (sIndex[0] == '\0')
	{
		sIndex[0] = '0';
	}


	ssPlayerId.fill(0);
	ssPlayerId.width(2);
	ssPlayerId << _gameScene->_localPlayerID;
	std::string sId = ssPlayerId.str();
	if (sId[0] == '\0')
	{
		sId[0] = '0';
	}

	return sIndex + sId;
}

void Manager::readRemoveBuildingCommand()
{
	std::string index(_command.begin() + 1, _command.begin() + 3);
	std::string playerId(_command.begin() + 3, _command.begin() + 5);

	_index = stringToNum<int>(index);
	_playerId = stringToNum<int>(playerId);
}

std::string Manager::getRemoveUnitMessage(Unit* u)
{
	//格式：索引 + 玩家id
	auto index = _gameScene->getSoldiers()->getIndex(u);

	std::stringstream ssIndex;
	std::stringstream ssPlayerId;

	ssIndex.fill(0);            //位宽为2，左侧补零
	ssIndex.width(2);
	ssIndex << index;
	std::string sIndex = ssIndex.str();
	if (sIndex[0] == '\0')
	{
		sIndex[0] = '0';
	}


	ssPlayerId.fill(0);
	ssPlayerId.width(2);
	ssPlayerId << _gameScene->_localPlayerID;
	std::string sId = ssPlayerId.str();
	if (sId[0] == '\0')
	{
		sId[0] = '0';
	}

	return sIndex + sId;
}

void Manager::readRemoveUnitCommand()
{
	std::string index(_command.begin() + 1, _command.begin() + 3);
	std::string playerId(_command.begin() + 3, _command.begin() + 5);

	_index = stringToNum<int>(index);
	_playerId = stringToNum<int>(playerId);
}

std::string Manager::getAttackMessage(Unit* u, int enemyId, int enemyIndex)
{
    //格式：玩家id + 索引 + enemyId + 索引
    std::stringstream ssPlayerId;
    std::stringstream ssPlayerIndex;
    std::stringstream ssEnemyId;
    std::stringstream ssEnemyIndex;

    cocos2d::log("get attack enemyIndex %d, enemyId %d", enemyIndex, enemyId);

    ssPlayerId.fill(0);
    ssPlayerId.width(2);
    ssPlayerId << _gameScene->_localPlayerID;
    std::string sId = ssPlayerId.str();
    if (sId[0] == '\0')
    {
        sId[0] = '0';
    }

    auto index = _gameScene->getSoldiers()->getIndex(u);
    ssPlayerIndex.fill(0);
    ssPlayerIndex.width(2);
    ssPlayerIndex << index;
    std::string sIndex = ssPlayerIndex.str();
    if (sIndex[0] == '\0')
    {
        sIndex[0] = '0';
    }

    ssEnemyId.fill(0);
    ssEnemyId.width(2);
    ssEnemyId << enemyId;
    std::string sEnemyId = ssEnemyId.str();
    if (sEnemyId[0] == '\0')
    {
        sEnemyId[0] = '0';
    }

    ssEnemyIndex.fill(0);
    ssEnemyIndex.width(2);
    ssEnemyIndex << enemyIndex;
    std::string sEnemyIndex = ssEnemyIndex.str();
    if (sEnemyIndex[0] == '\0')
    {
        sEnemyIndex[0] = '0';
    }

    return sId + sIndex + sEnemyId + sEnemyIndex;
}

void Manager::readAttackCommand()
{
    std::string playerId(_command.begin() + 1, _command.begin() + 3);
    std::string index(_command.begin() + 3, _command.begin() + 5);
    std::string enemyId(_command.begin() + 5, _command.begin() + 7);
    std::string enemyIndex(_command.begin() + 7, _command.begin() + 9);

    _index = stringToNum<int>(index);
    _playerId = stringToNum<int>(playerId);
    _enemyId = stringToNum<int>(enemyId);
    _enemyIndex = stringToNum<int>(enemyIndex);

    cocos2d::log("read command index %d, enemyIndex %d, enemyId %d", _index, _enemyIndex, _enemyId);
}

std::string Manager::getDeathMessage(int _playerId, int enemyId, int enemyIndex)
{
    //格式：命令发送玩家id + enemyId + 索引
    std::stringstream ssPlayerId;
    std::stringstream ssEnemyId;
    std::stringstream ssEnemyIndex;

    ssPlayerId.fill(0);
    ssPlayerId.width(2);
    ssPlayerId << _gameScene->_localPlayerID;
    std::string sId = ssPlayerId.str();
    if (sId[0] == '\0')
    {
        sId[0] = '0';
    }

    ssEnemyId.fill(0);
    ssEnemyId.width(2);
    ssEnemyId << enemyId;
    std::string sEnemyId = ssEnemyId.str();
    if (sEnemyId[0] == '\0')
    {
        sEnemyId[0] = '0';
    }

    ssEnemyIndex.fill(0);
    ssEnemyIndex.width(2);
    ssEnemyIndex << enemyIndex;
    std::string sEnemyIndex = ssEnemyIndex.str();
    if (sEnemyIndex[0] == '\0')
    {
        sEnemyIndex[0] = '0';
    }

    return sId + sEnemyId + sEnemyIndex;
}

void Manager::readDeathCommand()
{
    std::string playerId(_command.begin() + 1, _command.begin() + 3);
    std::string enemyId(_command.begin() + 3, _command.begin() + 5);
    std::string enemyIndex(_command.begin() + 5, _command.begin() + 7);

    _playerId = stringToNum<int>(playerId);
    _enemyId = stringToNum<int>(enemyId);
    _enemyIndex = stringToNum<int>(enemyIndex);
}