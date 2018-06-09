/*
*  @file     MoveController.cpp
*  @brief    各种兵的移动控制
*/
#include "MoveController.h"
#include "../Data/Building.h"
#include "../Data/UnitData.h"
#include <time.h>
#include <algorithm>

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
    // 检测是否障碍
    if (!_gameScene->isCollision(position))
    {
        return;
    }
 //===========假的垃圾队形===================
    int col;                     // 当前列
    int row = 0;                     // 当前行  
    int soldierSize = 40;            // To Do
    // 先放车
    col = 4;
    for (auto& car : *_selectedSoldiers)
    {
        if (car->getUnitTag() == TANK_TAG)
        {
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                col -= 2;              // 暂且认为坦克尺寸为兵的两倍 所以减2
                if (col < -4)
                {
                    col = 4;
                    row += 2;          // 暂且认为坦克尺寸为兵的两倍 所以加2
                }
            }
            car->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            // 寻路
            findRroute(car, car->_route);
            // 第一点
            car->setDestination(car->_route.front());
            (car->_route).erase((car->_route).begin());
            car->setGetDestination(false);
        }

        col -= 2;             // 暂且认为坦克尺寸为兵的两倍 所以减2
        if (col < -4)
        {
            col = 4;
            row += 2;          // 暂且认为坦克尺寸为兵的两倍 所以加2
        }
    }
    // 再放兵
    for (auto& soldier : *_selectedSoldiers)
    {
        switch (soldier->getUnitTag())
        {
        case TANK_TAG:
            continue;
        default:
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                --col;
                if (col < -3)
                {
                    col = 3;
                    ++row;
                }
            }
            soldier->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            break;
        }
        // 寻路
        findRroute(soldier, soldier->_route);
        // 第一点
        soldier->setDestination(soldier->_route.front());
        (soldier->_route).erase( (soldier->_route).begin() );
        soldier->setGetDestination(false);

        --col;
        if (col == -3)
        {
            col = 3;
            ++row;
        }
    }
}

void MoveController::moveSoldiers()
{
    static clock_t preT = clock();
    clock_t nowT = clock();
    float interval = nowT - preT;
    preT = nowT;
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (!soldier->getGetDestination())
        {
            Vec2 nowPosition = soldier->getPosition();
            Vec2 destination = soldier->getDestination();
            Vec2 direction = destination - nowPosition;
            direction.normalize();
            float distance = destination.distance(nowPosition);
         
            Vec2 move = soldier->getUnitSpeed() * interval * direction;
            // 如果move的距离大于到达目的地的距离则直接到达目的地的
            if (move.length() > distance)
            {
                soldier->moveTo(destination, 0.01);
                soldier->setGetDestination(true);
                continue;
            }
            soldier->moveTo(move + nowPosition, 0.01);
        }
        else if (soldier->_route.size())
        {
            soldier->setDestination(soldier->_route.front());
            (soldier->_route).erase((soldier->_route).begin());
            soldier->setGetDestination(false);
        }
    }
}

//用于寻路的节点
//whereX,whereY,当前士兵位置，以地图左下为原点
//cost 花费
//father 父节点，用于找到路后返回
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

//寻路算法的组成部分
//判断当前找到的地和目的地间是否有障碍
//如果没有障碍，结束寻路
//前面的路按找到的路走，后面的路直接走直线
//传入当前位置，目的地（地图坐标
bool MoveController::is_find(Vec2 position, Vec2 destination)
{
    Vec2 direction = destination - position;
    //两点之间任取15个点，如果都没障碍，即没障碍
    //因为地图特殊所以精度应该足够  
    for (int i = 0; i < 15; ++i)
    {
        if (!_gameScene->isCollision(_gameScene->
            _tileMap->convertToWorldSpace(position + i * direction / 20)))
        {
            return false;
        }
    }

    return true;
}

/**
寻路算法主体
类似A*算法
可参考 https://blog.csdn.net/jialeheyeshu/article/details/53105810
* @brief 传入要移动的士兵和一个Vector<Point*> route,来装找到的路
为了效率考虑，相邻两个节点的distance=240,，可以通过除以速度得到每秒以及每帧的位置
by czd
* @return  void
*/
void MoveController::findRroute(Unit *soldier, std::vector<Point> &route)
{
    Vec2 screenNowPosition = soldier->getPosition();
    Vec2 screenDestination = soldier->getDestination();
    //转化为以地图左下角为原点的坐标    
    Vec2 nowPosition = _gameScene->_tileMap->convertToNodeSpace(screenNowPosition);
    Vec2 nowDestination = _gameScene->_tileMap->convertToNodeSpace(screenDestination);
    //这个以后可能有用
    //struct cmp {
    //	bool operator()(node *a, node *b) {
    //		return a->cost >= b->cost;
    //	}
    //};
    std::queue<node*> open;
    std::vector<node*> storeNew;
    //std::priority_queue<node*, std::vector<node*>, cmp> close;
    node *head = new node(nowPosition.x, nowPosition.y, 0, NULL);
    storeNew.push_back(head);
    node *myend = head;
    open.push(head);
    int distance = 240;
    //8个方向
    //上下左右，左上左下等
    float directX[8] = { 0,0,-1 * distance ,distance,0.7*distance ,0.7* distance ,-0.7* distance ,-0.7* distance };
    float directY[8] = { distance,-1 * distance,0 ,0,0.7* distance ,-0.7* distance ,0.7* distance ,-0.7* distance };
    int is_not_find = 1;
    //这两个点间无障碍
    if (is_find(nowPosition, nowDestination))
    {
        is_not_find = 0;
    }
    while (is_not_find)
    {
        node *cur = open.front();
        //close.push(cur);
        open.pop();
        //列举8个方向        
        for (int i = 0; i < 8; i++)
        {
            //绑定位置是否有障碍            
            if (_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace
            (Point(cur->whereX + directX[i], cur->whereY + directY[i]))))
            {
                //进一步筛选                
                if (Vec2(nowDestination.x - (cur->whereX), nowDestination.y - (cur->whereY)).length() >
                    Vec2(nowDestination.x - (cur->whereX) - directX[i], nowDestination.y - (cur->whereY) - directY[i]).length()
                    - 0.7*distance)
                {
                    node *n = new node(cur->whereX + directX[i], cur->whereY + directY[i], distance, cur);
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
    //该地和目的地可直达
    Point nowPlace{ 0,0 };
    nowPlace.x = static_cast<float>(myend->whereX);
    nowPlace.y = static_cast<float>(myend->whereY);
    Point direction = nowDestination - nowPlace;
    direction.normalize();
    //从找到的地到出发地
    while (myend != NULL)
    {
        route.push_back(Point(myend->whereX, myend->whereY));
        myend = myend->father;
    }
    //reverse后，就是出发地到找到地
    std::reverse(route.begin(), route.end());
    //再把后面的路加入route
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
    // 转化为世界坐标
    std::vector<Point>::iterator iter2;
    for (iter2 = route.begin(); iter2 != route.end(); iter2++)
    {
        *iter2 = _gameScene->_tileMap->convertToWorldSpace(*iter2);
    }
}

bool MoveController::canPut(cocos2d::Point position)
{
    Vec2 mapPosition = _gameScene->_tileMap->convertToNodeSpace(position);
    // 是否在地图外
    if (mapPosition.x < 0 || mapPosition.y < 0)
    {
        return false;
    }
    // 是否在海上
    if (!_gameScene->isCollision(convertToWorldSpace(mapPosition)))
    {
        return false;
    }
    // 是否有建筑士兵
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        Rect rect = Rect(soldier->getPositionX() - soldier->getContentSize().width / 2,
            soldier->getPositionY() - soldier->getContentSize().height / 2,
            soldier->getContentSize().width, soldier->getContentSize().height);
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
    // 都没有
    return true;
}