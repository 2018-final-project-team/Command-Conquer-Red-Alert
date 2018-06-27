/*
*  @file     GameManager.cpp
*  @brief    游戏的控制类
*/

#include "GameManager.h"
#include "Scene/EndingScene.h"

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

    _selectedEnemyId = _selectedEnemyIndex = 0;
    _selectedBuildingId = _selectedBuildingIndex = 0;

    std::memset(_isUnitDied, 0, sizeof(_isUnitDied));
    std::memset(_isBuildingDied, 0, sizeof(_isBuildingDied));

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
		case SATELLITE_TAG:
			costMoney = buildingData::satelliteCostMoney;
			if (_gameScene->getIsPowerEnough())
			{
				_waitTimeToCreateBuilding = buildingData::EnoughPower::satelliteWait;
			}
			else
			{
				_waitTimeToCreateBuilding = buildingData::NotEnoughPower::satelliteWait;
			}
			break;
		case DEFENSE_BUILDING_TAG:
			costMoney = buildingData::defenseBuildingCostMoney;
			if (_gameScene->getIsPowerEnough())
			{
				_waitTimeToCreateBuilding = buildingData::EnoughPower::defenseBuildingWait;
			}
			else
			{
				_waitTimeToCreateBuilding = buildingData::NotEnoughPower::defenseBuildingWait;
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
            Unit* soldier = Unit::create(_soldierTag, _gameScene->_localPlayerID, _gameScene->_unitIndex);
            ++_gameScene->_unitIndex;
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
            Unit* car = Unit::create(_carTag, _gameScene->_localPlayerID, _gameScene->_unitIndex);
            ++_gameScene->_unitIndex;
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

		Building* building;
		if (_buildingTag == DEFENSE_BUILDING_TAG)
		{
			building = DefenseBuilding::create(_buildingTag, _gameScene->_localPlayerID);
		}
		else
		{
			building = Building::create(_buildingTag, _gameScene->_localPlayerID, _gameScene->_buildingIndex);
            ++_gameScene->_buildingIndex;
		}
        
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
		case SATELLITE_TAG:
			_gameScene->addSatellite();
			costPower = buildingData::satelliteCostPower;
			break;
		case DEFENSE_BUILDING_TAG:
			costPower = buildingData::defenseBuildingCostPower;
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
        _selectedEnemyIndex = _selectedEnemy->getIndex();
    }
    else
    {
        _selectedEnemyId = _selectedEnemy->getID();
        _selectedEnemyIndex = _selectedEnemy->getIndex();
    }
}

void Manager::setBuilding(Building* building)
{
    _selectedBuilding = building;
    _selectedEnemy = nullptr;
    //移动攻击.
    if (_gameScene->getSelectedSoldiers()->size() > 0)
    {
        Vec2 direction = _gameScene->getSelectedSoldiers()->front()->getPosition()
            - building->getPosition();
        direction.normalize();
        _moveController->setDestination(building->getPosition() + direction * 80);
    }
    
    _selectedBuildingId = _selectedBuilding->getID();
    _selectedBuildingIndex = _gameScene->getEnemyBuildingsByID
            (_selectedBuildingId)->getIndex(_selectedBuilding);
    log("set building id %d index %d", _selectedBuildingId, _selectedBuildingIndex);
}

void Manager::attack()
{
    static clock_t infantryPreT = clock();
    static clock_t dogPreT = clock();
    static clock_t tankPreT = clock();
    static clock_t defensePreT = clock();
    bool isInfantryAttack = false;
    bool isDogAttack = false;
    bool isTankAttack = false;
    bool isDefenseAttack = false;
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
                    _gameScene->_client->sendMessage(ATTACK_UNIT, 
                        getAttackMessage(soldier, _selectedEnemyId, _selectedEnemyIndex));
                    //log("attack enemy id %d index %d", _selectedEnemyId, _selectedEnemyIndex);
  
                    isInfantryAttack = isSoldierAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitATKCD())
                {
                    _gameScene->_client->sendMessage(ATTACK_UNIT, 
                        getAttackMessage(soldier, _selectedEnemyId, _selectedEnemyIndex));

                    isDogAttack = isSoldierAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitATKCD())
                {
                    _gameScene->_client->sendMessage(ATTACK_UNIT, 
                        getAttackMessage(soldier, _selectedEnemyId, _selectedEnemyIndex));

                    isTankAttack = isSoldierAttack = true;
                }
                break;
            }

            continue;
        }
        else if (_selectedBuilding && soldier->canAttack(_selectedBuilding->getPosition()) && !isSoldierAttack)  // 如果选中了敌方建筑
        {
            switch (soldier->getUnitTag())
            {
            case INFANTRY_TAG:
                if (nowT - infantryPreT >= soldier->getUnitATKCD())
                {
                    _gameScene->_client->sendMessage(ATTACK_BUILDING,
                        getAttackMessage(soldier, _selectedBuildingId, _selectedBuildingIndex));

                    isInfantryAttack = isSoldierAttack = true;
                }
                break;
            case DOG_TAG:
                if (nowT - dogPreT >= soldier->getUnitATKCD())
                {
                    _gameScene->_client->sendMessage(ATTACK_BUILDING,
                        getAttackMessage(soldier, _selectedBuildingId, _selectedBuildingIndex));

                    isDogAttack = isSoldierAttack = true;
                }
                break;
            case TANK_TAG:
                if (nowT - tankPreT >= soldier->getUnitATKCD())
                {
                    _gameScene->_client->sendMessage(ATTACK_BUILDING,
                        getAttackMessage(soldier, _selectedBuildingId, _selectedBuildingIndex));

                    isTankAttack = isSoldierAttack = true;
                }
            }

            continue;
        }

        for (int id = 1; id <= 4 && !isSoldierAttack; ++id)
        {
            for (int i = 0; i < _gameScene->getEnemySoldiersByID(id)->size() && !isSoldierAttack; ++i)
            {
                auto enemy = _gameScene->getEnemySoldiersByID(id)->at(i);

                if (soldier->canAttack(enemy->getPosition()))
                {
                    switch (soldier->getUnitTag())
                    {
                    case INFANTRY_TAG:
                        if (nowT - infantryPreT >= soldier->getUnitATKCD())
                        {
                            _gameScene->_client->sendMessage(ATTACK_UNIT,
                                getAttackMessage(soldier, id, enemy->getIndex()));

                            isInfantryAttack = isSoldierAttack = true;
                        }
                        break;
                    case DOG_TAG:
                        if (nowT - dogPreT >= soldier->getUnitATKCD())
                        {
                            _gameScene->_client->sendMessage(ATTACK_UNIT,
                                getAttackMessage(soldier, id, enemy->getIndex()));

                            isDogAttack = isSoldierAttack = true;
                        }
                        break;
                    case TANK_TAG:
                        if (nowT - tankPreT >= soldier->getUnitATKCD())
                        {
                            _gameScene->_client->sendMessage(ATTACK_UNIT,
                                getAttackMessage(soldier, id, enemy->getIndex()));

                            isTankAttack = isSoldierAttack = true;
                        }
                        break;
                    }
                    break;
                }
            }
        }
    }

    for (int i = 0; i < _gameScene->getBuildings()->size(); ++i)
    {
        auto building = _gameScene->getBuildings()->at(i);
        if (building->getBuildingTag() == DEFENSE_BUILDING_TAG && 
            nowT - defensePreT >= 1000)
        {
            auto soldier = static_cast<DefenseBuilding*>(building);
            bool att = false;
            for (int id = 1; id <= 4 && !att; ++id)
            {
                for (int j = 0; j < _gameScene->getEnemySoldiersByID(id)->size(); ++j)
                {
                    auto enemy = _gameScene->getEnemySoldiersByID(id)->at(j);

                    if (soldier->canAttack(enemy->getPosition()))
                    {
                        _gameScene->_client->sendMessage
                        (DEFENSE_ATTACK, getDefenseAttackMessage(i, id, enemy->getIndex()));
                        att = true;
                        isDefenseAttack = true;
                        break;
                    }
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
    if (isDefenseAttack)
    {
        defensePreT = nowT;
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
	if (totalPower - castPower >= 0)
	{
		_gameScene->setIsPowerEnough(true);
	}
	else
	{
		_gameScene->setIsPowerEnough(false);
	}
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

			Building* building;
			if (buildingTag == DEFENSE_BUILDING_TAG)
			{
				building = DefenseBuilding::create(buildingTag, _playerId);
			}
			else
			{
				building = Building::create(buildingTag, _playerId, _gameScene->_buildingIndex);
                ++_gameScene->_buildingIndex;
			}

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
			Unit* soldier = Unit::create(_tagForMessage, _playerId, _gameScene->_unitIndex);
            ++_gameScene->_unitIndex;
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
            _gameScene->_buildingIndexDied[_gameScene->
                getEnemyBuildingsByID(_playerId)->at(_index)->getIndex()] = 1;
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
            _gameScene->_unitIndexDied[_gameScene->getEnemySoldiersByID(_playerId)->at(_index)->getIndex()] = 1;
			_gameScene->removeChild(_gameScene->getEnemySoldiersByID(_playerId)->at(_index));
			_gameScene->getEnemySoldiersByID(_playerId)->erase(_index);
		}
        else if (_command[0] == ATTACK_UNIT[0])
        {
            readAttackCommand();
            Unit* soldier;
            Unit* enemy;
            //====================get attacker through index========================
            //cocos2d::log("get attacker playerId %d index %d", _playerId, _index);
            if (_playerId == _gameScene->_localPlayerID)
            {
                soldier = _gameScene->getSoldierByIndex(_index);
            }
            else
            {
                soldier = _gameScene->getEnemySoldierByIdIndex(_playerId, _index);
            }
            //=================get enemy through index==================
            if (_enemyId == _gameScene->_localPlayerID)
            {
                enemy = _gameScene->getSoldierByIndex(_enemyIndex);
            }
            else
            {
                enemy = _gameScene->getEnemySoldierByIdIndex(_enemyId, _enemyIndex);
            }
            //cocos2d::log("get enemy id %d index %d", _enemyId, _enemyIndex);
            //=======================attack=================================
            if (enemy && soldier)
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
                    if (_selectedEnemyId == _enemyId && _selectedEnemyIndex == _enemyIndex)
                    {
                        _selectedEnemy = nullptr;
                        _selectedEnemyId = _selectedEnemyIndex = 0;
                    }
                    if (_gameScene->_localPlayerID == _enemyId)
                    {
                        _gameScene->removeChild(enemy);
                        _gameScene->getSoldiers()->eraseObject(enemy);
                    }
                    else
                    {
                        _gameScene->removeChild(enemy);
                        _gameScene->getEnemySoldiersByID(_enemyId)->eraseObject(enemy);
                    }
                }
            }
        }
        else if (_command[0] == ATTACK_BUILDING[0])
        {
            readAttackCommand();
            if (_isBuildingDied[_enemyId][_enemyIndex])
            {
                continue;
            }
            Unit* soldier;
            Building* enemy;
            //====================get attacker through index========================
            //cocos2d::log("get attacker playerId %d index %d", _playerId, _index);
            if (_playerId == _gameScene->_localPlayerID)
            {
                soldier = _gameScene->getSoldierByIndex(_index);
            }
            else
            {
                soldier = _gameScene->getEnemySoldierByIdIndex(_playerId, _index);
            }
            
            //=================get enemy through index==================
            if (_enemyId == _gameScene->_localPlayerID)
            {
                enemy = _gameScene->getBuildings()->at(_enemyIndex);
            }
            else
            {
                enemy = _gameScene->getEnemyBuildingsByID(_enemyId)->at(_enemyIndex);
            }

            //cocos2d::log("get enemy id %d index %d", _enemyId, _enemyIndex);
            //=======================attack=================================
            if (soldier)
            {
                if (soldier)
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

                    if (enemy->getHP() <= 0)
                    {
                        _isBuildingDied[_enemyId][_enemyIndex] = 1;
                        if (_gameScene->_localPlayerID == enemy->getID())
                        {
                            _gameScene->getBuildings()->erase(_enemyIndex);
                            buildingDied(enemy->getBuildingTag());
                        }
                        else
                        {
                            _gameScene->getEnemyBuildingsByID(_enemyId)->erase(_enemyIndex);
                        }
                        _gameScene->removeChild(enemy);
                        if (_gameScene->_localPlayerID == 2)
                        {
                            _gameScene->_client->sendMessage(BUILDING_DIED, getDeathMessage(_enemyId, _enemyIndex));
                        }
                        if (_selectedBuildingId == _enemyId && _selectedBuildingIndex == _enemyIndex)
                        {
                            _selectedBuilding = nullptr;
                            _selectedBuildingId = _selectedBuildingIndex = 0;
                        }
                    }
                    else
                    {
                        if (enemy->getBuildingTag() == POWER_PLANT_TAG)
                        {
                            resetPower();
                        }
                    }
                }
            
            }
        }
        else if (_command[0] == DEFENSE_ATTACK[0])
        {
            readDefenseAttackCommand();
            if (_isBuildingDied[_playerId][_index])
            {
                continue;
            }
            DefenseBuilding* soldier;
            Unit* enemy;
            //================get soldier=============
            if (_playerId == _gameScene->_localPlayerID)
            {
                soldier = static_cast<DefenseBuilding*>(_gameScene->getBuildings()->at(_index));
            }
            else
            {
                soldier = static_cast<DefenseBuilding*>
                    (_gameScene->getEnemyBuildingsByID(_playerId)->at(_index));
            }
            //==============get enemy=================
            if (_enemyId == _gameScene->_localPlayerID)
            {
                enemy = _gameScene->getSoldierByIndex(_enemyIndex);
            }
            else
            {
                enemy = _gameScene->getEnemySoldierByIdIndex(_enemyId, _enemyIndex);
            }
            //==============attack===================
            if (enemy)
            {
                soldier->attack(enemy);

                if (enemy->getUnitHP() <= 0)
                {
                    enemy->switchState(stateDeath);
                    if (_gameScene->_localPlayerID == enemy->getID())
                    {
                        _gameScene->getSoldiers()->eraseObject(enemy);
                    }
                    else
                    {
                        _gameScene->getEnemySoldiersByID(_enemyId)->eraseObject(enemy);
                    }
                    _gameScene->removeChild(enemy);
                    if (_selectedEnemyId == _enemyId && _selectedEnemyIndex == _enemyIndex)
                    {
                        _selectedEnemy = nullptr;
                        _selectedEnemyId = _selectedEnemyIndex = 0;
                    }
                }
            }
        }
        else if (_command[0] == UNIT_DIED[0])
        {
            readDeathCommand();
            _isUnitDied[_enemyId][_enemyIndex] = 0;
            
            log("unit die enemyid %d enemyindex %d", _enemyId, _enemyIndex);
        }
        else if (_command[0] == BUILDING_DIED[0])
        {
            readDeathCommand();
            _isBuildingDied[_enemyId][_enemyIndex] = 0;
        }
		else if (_command[0] == DEAD_MESSAGE[0])
		{
			std::string sId(_command.begin() + 1, _command.begin() + 1);
			std::stringstream ssId;
			ssId << sId;
			int id;
			ssId >> id;
			if (id != _gameScene->_localPlayerID)
			{
				_gameScene->_loserCnt++;
				if (_gameScene->_loserCnt == _gameScene->_inputData->player_list.size() - 1)
				{

					_gameScene->_gameEventDispatcher->removeAllEventListeners();
					//释放定时器
					this->unscheduleUpdate();
					this->unscheduleAllSelectors();

					Director::getInstance()->replaceScene(TransitionFade::create(1, EndingScene::createScene(true)));
				}
			}
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
    //格式：玩家id + index + enemyId + index
    std::stringstream ssPlayerId;
    std::stringstream ssPlayerIndex;
    std::stringstream ssEnemyId;
    std::stringstream ssEnemyIndex;

    //cocos2d::log("get attack enemyIndex %d, enemyId %d", enemyIndex, enemyId);

    ssPlayerId.fill(0);
    ssPlayerId.width(2);
    ssPlayerId << _gameScene->_localPlayerID;
    std::string sId = ssPlayerId.str();
    if (sId[0] == '\0')
    {
        sId[0] = '0';
    }

    auto index = u->getIndex();
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

    //cocos2d::log("read command index %d, enemyIndex %d, enemyId %d", _index, _enemyIndex, _enemyId);
}

std::string Manager::getDefenseAttackMessage(int index, int enemyId, int enemyIndex)
{
    //格式：玩家id + 索引 + enemyId + 索引
    std::stringstream ssPlayerId;
    std::stringstream ssPlayerIndex;
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

    log("get defense attack playerid %d index %d enemyid %d enemyindex %d", 
        _gameScene->_localPlayerID, index, enemyId, enemyIndex);

    return sId + sIndex + sEnemyId + sEnemyIndex;
}

void Manager::readDefenseAttackCommand()
{
    std::string playerId(_command.begin() + 1, _command.begin() + 3);
    std::string index(_command.begin() + 3, _command.begin() + 5);
    std::string enemyId(_command.begin() + 5, _command.begin() + 7);
    std::string enemyIndex(_command.begin() + 7, _command.begin() + 9);

    _index = stringToNum<int>(index);
    _playerId = stringToNum<int>(playerId);
    _enemyId = stringToNum<int>(enemyId);
    _enemyIndex = stringToNum<int>(enemyIndex);

    log("read defense attack playerid %d index %d enemyid %d enemyindex %d",
        _playerId, _index, _enemyId, _enemyIndex);
}

std::string Manager::getDeathMessage(int enemyId, int enemyIndex)
{
    //格式：enemyId + 索引
    std::stringstream ssEnemyId;
    std::stringstream ssEnemyIndex;

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

    return sEnemyId + sEnemyIndex;
}

void Manager::readDeathCommand()
{
    std::string enemyId(_command.begin() + 1, _command.begin() + 3);
    std::string enemyIndex(_command.begin() + 3, _command.begin() + 5);

    _enemyId = stringToNum<int>(enemyId);
    _enemyIndex = stringToNum<int>(enemyIndex);
}