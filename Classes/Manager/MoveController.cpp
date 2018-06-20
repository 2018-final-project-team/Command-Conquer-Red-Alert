/*
*  @file     MoveController.cpp
*  @brief    MoveController
*/
#include "MoveController.h"
#include "../Data/Building.h"
#include "../Data/UnitData.h"
#include <time.h>
#include <algorithm>
#include <sstream>
#include <string>


USING_NS_CC;

MoveController* MoveController::createWithGameScene(GameScene* gameScene)
{
    MoveController* move_controller = new MoveController();
    if (move_controller->initWithGameScene(gameScene))
    {
        move_controller->autorelease();
        return move_controller;
    }

    CC_SAFE_DELETE(move_controller);
    return nullptr;
}

bool MoveController::initWithGameScene(GameScene* gameScene)
{
    _selectedSoldiers = gameScene->getSelectedSoldiers();
    _gameScene = gameScene;

    return true;
}

void MoveController::selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint)
{
	for (Unit* unit : *_selectedSoldiers)
	{
		unit->setIsSelected(false);
	}
    _selectedSoldiers->clear();
    float rect_width = fabs(mouseUpPoint.x - mouseDownPoint.x);
    float rect_height = fabs(mouseUpPoint.y - mouseDownPoint.y);
    Rect rect(MIN(mouseDownPoint.x, mouseUpPoint.x), MIN(mouseDownPoint.y, mouseUpPoint.y), rect_width, rect_height);
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (rect.containsPoint(soldier->getPosition()))
        {
            soldier->setIsSelected(true);
            _selectedSoldiers->pushBack(soldier);
        }
    }
}

void MoveController::selectSoldiersWithTag(Tag tag)
{
	for (Unit* unit : *_selectedSoldiers)
	{
		unit->setIsSelected(false);
	}
    _selectedSoldiers->clear();
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (soldier->getTag() == tag)
        {
            soldier->setIsSelected(true);
            _selectedSoldiers->pushBack(soldier);
        }
    }
}

void MoveController::setDestination(cocos2d::Vec2 position)
{
 //===========laji duixin===================
    int col = 4;                     // column of duixin
    int row = 0;                     // row of duixin
    int soldierSize = 30;            // To Do
    int carNum = 0;
    // put the cars first   
    for (auto& car : *_selectedSoldiers)
    {
        if (car->getUnitTag() == TANK_TAG)
        {
            carNum++;
        }
    }
    if (carNum == 2)
    {
        col = 2;
    }
    else if (carNum == 1)
    {
        col = 0;
    }
    for (auto& car : *_selectedSoldiers)
    {
        if (car->getUnitTag() == TANK_TAG)
        {
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                col -= 2;              
                if (col < -4)
                {
                    col = 4;
                    row += 1;          
                }
                //log("%d %d", col, row);
            }
            car->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            //log("%d %d car %f %f", col, row, car->getDestination().x, car->getDestination().y);
            // find the best way
            findRroute(car, car->_route);
            car->setDestination(car->_route.front());
            //just for change direction halfway
            car->setGetDestination(true);

            col -= 2;             // temporarily think car is double size than infantry
            if (col < -4)
            {
                col = 4;
                row += 1;          // temporarily think car is double size than infantry
            }
            carNum++;
        }
    }
    // then put the soldiers   
    int maxCol = 3;
    if (!carNum && _selectedSoldiers->size() <= 3)
    {
        col = 1;
        maxCol = 1;
    }
    for (auto& soldier : *_selectedSoldiers)
    {
        switch (soldier->getUnitTag())
        {
        case TANK_TAG:
            continue;
        case BASE_CAR_TAG:
        {
            soldier->setDestination(position);
            soldier->setGetDestination(false);

            _gameScene->_client->sendMessage(MOVE_UNIT, getMoveMessage(soldier));

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
            return;
        }
        default:
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                --col;
                if (col < -maxCol)
                {
                    col = maxCol;
                    ++row;
                }
                //log("%d %d", col, row);
            }
            soldier->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            //log("%d %d soldier %f %f",col, row, soldier->getDestination().x, soldier->getDestination().y);
            // find the best way
            findRroute(soldier, soldier->_route);
            soldier->setDestination(soldier->_route.front());
            //just for change direction halfway
            soldier->setGetDestination(true);

            --col;
            if (col < -maxCol)
            {
                col = maxCol;
                ++row;
            }

            break;
        }
    }
}

void MoveController::moveSoldiers()
{
    static clock_t preT = clock();
    clock_t nowT = clock();
    float interval = nowT - preT;
    preT = nowT;
    //my soldiers
    Unit* soldier;
    for (int i = 0; i < _gameScene->getSoldiers()->size(); ++i)
    {
        soldier = _gameScene->getSoldiers()->at(i);
        if (!soldier->getGetDestination())
        {
            Vec2 nowPosition = soldier->getPosition();
            Vec2 destination = soldier->getDestination();
            Vec2 direction = destination - nowPosition;
            direction.normalize();
            float distance = destination.distance(nowPosition);
            //log("now position %f %f", nowPosition.x, nowPosition.y);
            //log("destination %f %f", destination.x, destination.y);
         
            Vec2 move = soldier->getUnitSpeed() * interval * direction;
            // if the distance of this move is longer than destination
            if (move.length() > distance)
            {
                soldier->moveTo(destination);
                soldier->setGetDestination(true);
                soldier->switchState(stateNone);
                continue;
            }
            soldier->moveTo(move + nowPosition);
        }
        else if (soldier->_route.size())
        {
            //log("route size %d", soldier->_route.size());
            soldier->setDestination(soldier->_route.front());
            _gameScene->_client->sendMessage(MOVE_UNIT, getMoveMessage(soldier));
            (soldier->_route).erase((soldier->_route).begin());
            soldier->setGetDestination(false);
            Vec2 direction= (soldier->getDestination() - soldier->getPosition());
            log("destination %f %f", soldier->getDestination().x, soldier->getDestination().y);
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
        }
    }
    //enemy
    for (auto& enemy : *(_gameScene->getEnemySoldiers()))
    {
        if (enemy->getGetDestination())
        {
            Vec2 nowPosition = enemy->getPosition();
            Vec2 destination = enemy->getDestination();
            Vec2 direction = destination - nowPosition;
            direction.normalize();
            float distance = destination.distance(nowPosition);
            //log("now position %f %f", nowPosition.x, nowPosition.y);
            //log("destination %f %f", destination.x, destination.y);

            Vec2 move = enemy->getUnitSpeed() * interval * direction;
            // if the distance of this move is longer than destination
            if (move.length() > distance)
            {
                enemy->moveTo(destination);
                enemy->setGetDestination(true);
                enemy->switchState(stateNone);
                continue;
            }
            enemy->moveTo(move + nowPosition);
        }
    }
}

//i forget what is this
//what's
//the
//fuck
struct node 
{
    int whereX;
    int whereY;
    int cost;
    node* father;
    node(int wx, int wy, int c, node* f) 
    {
        whereX = wx;
        whereY = wy;
        cost = c;
        father = f;
    }
};

//what
//is
//the
//fuck
//i,m dying
bool MoveController::is_find(Vec2 position, Vec2 destination)
{
    Vec2 direction = destination - position;
    //i
    //am 
    for (int i = 0; i < 25; ++i)
    {
        if (_gameScene->isCollision(_gameScene->
            _tileMap->convertToWorldSpace(position + i * direction / 25)))
        {
            return false;
        }
    }

    return true;
}

/**
going
to
die https://blog.csdn.net/jialeheyeshu/article/details/53105810
* @brief you can read more about the algorithm by clicking this url
and this function is written by czd
* @return  void
*/
void MoveController::findRroute(Unit *soldier, std::vector<Point> &route)
{
    int mapNode[200][200];
	memset(mapNode, 0, sizeof(mapNode));
    route.clear();
    Vec2 screenNowPosition = soldier->getPosition();
    Vec2 screenDestination = soldier->getDestination();
    //so  
    Vec2 nowPosition = _gameScene->_tileMap->convertToNodeSpace(screenNowPosition);
    Vec2 nowDestination = _gameScene->_tileMap->convertToNodeSpace(screenDestination);
    std::queue<node*> open;
    std::vector<node*> storeNew;
    //std::priority_queue<node*, std::vector<node*>, cmp> close;
    node *head = new node(nowPosition.x, nowPosition.y, 0, NULL);
    storeNew.push_back(head);
    node *myend = head;
    open.push(head);
    int distance = 100;
    //don't    
    //know
    float directX[8] = { 0.7*distance ,0.7* distance ,-0.7* distance ,-0.7* distance ,0,0,-1 * distance ,distance };
    float directY[8] = { 0.7* distance ,-0.7* distance ,0.7* distance ,-0.7* distance,distance,-1 * distance,0 ,0 };
    int is_not_find = 1;
    //whats
    if (is_find(nowPosition, nowDestination))
    {
        is_not_find = 0;
    }
    while (is_not_find)
    {
        node *cur = open.front();
        //close.push(cur);
        open.pop();
        //is      
        for (int i = 0; i < 8; i++)
        {
            //this         
            if (!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(Point(cur->whereX + directX[i], cur->whereY + directY[i])))&&
				!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(Point(cur->whereX + directX[i]/4, cur->whereY + directY[i]/4)))&&
				!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(Point(cur->whereX + directX[i] / 2, cur->whereY + directY[i] / 2)))&&
				!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(Point(cur->whereX + directX[i]*3 / 4, cur->whereY + directY[i]*3 / 4))))
            {
				int nodeX = (cur->whereX + directX[i])*3.5 / distance;
				int nodeY = (cur->whereY + directY[i])*3.5 / distance;
                //fucking           
                if (mapNode[nodeX][nodeY]!=1)
                {
                    node *n = new node(cur->whereX + directX[i], cur->whereY + directY[i], distance, cur);
					mapNode[nodeX][nodeY] = 1;
                    storeNew.push_back(n);
                    if (is_find(Point(n->whereX, n->whereY), nowDestination))
                    {
                        is_not_find = 0;
                        myend = n;
                        break;
                    }
                    open.push(n);
                }
            }

        }
    }
    //notes
    Point nowPlace{ 0,0 };
    nowPlace.x = static_cast<float>(myend->whereX);
    nowPlace.y = static_cast<float>(myend->whereY);
    Point direction = nowDestination - nowPlace;
    direction.normalize();
    //let   
    while (myend != NULL)
    {
        route.push_back(Point(myend->whereX, myend->whereY));
        myend = myend->father;
    }
    //the 
    std::reverse(route.begin(), route.end());
    //fucking
    while ((nowDestination - nowPlace).length() > distance)
    {
        route.push_back(nowPlace + distance * direction);
        nowPlace += distance * direction;
    }
    route.push_back(nowDestination);
    // delete
    std::vector<node*>::iterator iter = storeNew.end() - 1;
    size_t i = storeNew.size();
    for (size_t cnt = 0; cnt < i; cnt++)
    {
        delete *iter;
        if (cnt < i - 1)
        {
            iter--;
        }
    }
    //vs  
    std::vector<Point>::iterator iter2;
    for (iter2 = route.begin(); iter2 != route.end(); iter2++)
    {
        *iter2 = _gameScene->_tileMap->convertToWorldSpace(*iter2);
    }
}

bool MoveController::canPut(cocos2d::Point position)
{
    // is the collision or not in map
    if (_gameScene->isCollision(position))
    {
        return false;
    }
    // is there any soldier or building
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        //log("soldier position %f %f", soldier->getPosition().x, soldier->getPosition().y);
        Rect rect = Rect(soldier->getPositionX() - soldier->getContentSize().width / 2,
            soldier->getPositionY() - soldier->getContentSize().height / 2,
            soldier->getContentSize().width, soldier->getContentSize().height);
        //log("%f %f %f %f", rect.getMinX(), rect.getMinY(), rect.size.width, rect.size.height);
        //log("put position %f %f", position.x, position.y);
        if (rect.containsPoint(position))
        {
            return false;
        }
    }
    for (auto& building : *(_gameScene->getBuildings()))
    {
        Rect rect = Rect(building->getPositionX() - building->getContentSize().width / 2,
            building->getPositionY() - building->getContentSize().height / 2,
            building->getContentSize().width, building->getContentSize().height);
        if (rect.containsPoint(position))
        {
            return false;
        }
    }
    // yes you can put soldier here   
    return true;
}

std::string MoveController::getMoveMessage(Unit* u)
{
	//格式：索引 + 玩家name + (X,Y)
	auto index = _gameScene->getSoldiers()->getIndex(u);

	std::stringstream ssIndex;
	std::stringstream ssX;
	std::stringstream ssY;

	std::string s1 = "(";
	std::string s2 = ",";
	std::string s3 = ")";

	ssIndex.fill(0);            //索引位宽为2，左侧补零
	ssIndex.width(2);
	ssIndex << index;
	std::string sIndex = ssIndex.str();
	if (sIndex[0] == '\0')
	{
		sIndex[0] = '0';
	}

    auto mapPosition = _gameScene->_tileMap->convertToNodeSpace(u->getDestination());
	
	ssX << mapPosition.x;
	std::string sX = ssX.str();

	ssY << mapPosition.y;
	std::string sY = ssY.str();

	return sIndex + _gameScene->_localPlayerName + s1 + sX + s2 + sY + s3;
}