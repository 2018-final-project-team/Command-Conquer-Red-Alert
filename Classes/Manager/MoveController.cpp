/*
*  @file     MoveController.cpp
*  @brief    ���ֱ����ƶ�����
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
    // ����Ƿ��ϰ�
    if (_gameScene->isCollision(position))
    {
        return;
    }
 //===========�ٵ���������===================
    int col;                     // ��ǰ��
    int row = 0;                     // ��ǰ��  
    int soldierSize = 40;            // To Do
    // �ȷų�
    col = 4;
    for (auto& car : *_selectedSoldiers)
    {
        if (car->getUnitTag() == TANK_TAG)
        {
            while (!canPut(position + Vec2(col * soldierSize, -row * soldierSize)))
            {
                col -= 2;              // ������Ϊ̹�˳ߴ�Ϊ�������� ���Լ�2
                if (col < -4)
                {
                    col = 4;
                    row += 2;          // ������Ϊ̹�˳ߴ�Ϊ�������� ���Լ�2
                }
            }
            car->setDestination(position + Vec2(col * soldierSize, -row * soldierSize));
            // Ѱ·
            findRroute(car, car->_route);
            // ��һ��
            car->setDestination(car->_route.front());
            (car->_route).erase((car->_route).begin());
            car->setGetDestination(false);
        }

        col -= 2;             // ������Ϊ̹�˳ߴ�Ϊ�������� ���Լ�2
        if (col < -4)
        {
            col = 4;
            row += 2;          // ������Ϊ̹�˳ߴ�Ϊ�������� ���Լ�2
        }
    }
    // �ٷű�
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
        // Ѱ·
        findRroute(soldier, soldier->_route);
        // ��һ��
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
            // ���move�ľ�����ڵ���Ŀ�ĵصľ�����ֱ�ӵ���Ŀ�ĵص�
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

//����Ѱ·�Ľڵ�
//whereX,whereY,��ǰʿ��λ�ã��Ե�ͼ����Ϊԭ��
//cost ����
//father ���ڵ㣬�����ҵ�·�󷵻�
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

//Ѱ·�㷨����ɲ���
//�жϵ�ǰ�ҵ��ĵغ�Ŀ�ĵؼ��Ƿ����ϰ�
//���û���ϰ�������Ѱ·
//ǰ���·���ҵ���·�ߣ������·ֱ����ֱ��
//���뵱ǰλ�ã�Ŀ�ĵأ���ͼ����
bool MoveController::is_find(Vec2 position, Vec2 destination)
{
    Vec2 direction = destination - position;
    //����֮����ȡ15���㣬�����û�ϰ�����û�ϰ�
    //��Ϊ��ͼ�������Ծ���Ӧ���㹻  
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
Ѱ·�㷨����
����A*�㷨
�ɲο� https://blog.csdn.net/jialeheyeshu/article/details/53105810
* @brief ����Ҫ�ƶ���ʿ����һ��Vector<Point*> route,��װ�ҵ���·
Ϊ��Ч�ʿ��ǣ����������ڵ��distance=240,������ͨ�������ٶȵõ�ÿ���Լ�ÿ֡��λ��
by czd
* @return  void
*/
void MoveController::findRroute(Unit *soldier, std::vector<Point> &route)
{
    Vec2 screenNowPosition = soldier->getPosition();
    Vec2 screenDestination = soldier->getDestination();
    //ת��Ϊ�Ե�ͼ���½�Ϊԭ�������    
    Vec2 nowPosition = _gameScene->_tileMap->convertToNodeSpace(screenNowPosition);
    Vec2 nowDestination = _gameScene->_tileMap->convertToNodeSpace(screenDestination);
    //����Ժ��������
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
    //8������
    //�������ң��������µ�
    float directX[8] = { 0,0,-1 * distance ,distance,0.7*distance ,0.7* distance ,-0.7* distance ,-0.7* distance };
    float directY[8] = { distance,-1 * distance,0 ,0,0.7* distance ,-0.7* distance ,0.7* distance ,-0.7* distance };
    int is_not_find = 1;
    //������������ϰ�
    if (is_find(nowPosition, nowDestination))
    {
        is_not_find = 0;
    }
    while (is_not_find)
    {
        node *cur = open.front();
        //close.push(cur);
        open.pop();
        //�о�8������        
        for (int i = 0; i < 8; i++)
        {
            //��λ���Ƿ����ϰ�            
            if (_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace
            (Point(cur->whereX + directX[i], cur->whereY + directY[i]))))
            {
                //��һ��ɸѡ                
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
    //�õغ�Ŀ�ĵؿ�ֱ��
    Point nowPlace{ 0,0 };
    nowPlace.x = static_cast<float>(myend->whereX);
    nowPlace.y = static_cast<float>(myend->whereY);
    Point direction = nowDestination - nowPlace;
    direction.normalize();
    //���ҵ��ĵص�������
    while (myend != NULL)
    {
        route.push_back(Point(myend->whereX, myend->whereY));
        myend = myend->father;
    }
    //reverse�󣬾��ǳ����ص��ҵ���
    std::reverse(route.begin(), route.end());
    //�ٰѺ����·����route
    while ((nowDestination - nowPlace).length() > distance)
    {
        route.push_back(nowPlace + distance * direction);
        nowPlace += distance * direction;
    }
    route.push_back(nowDestination);
    // delete
    std::vector<node*>::iterator iter;
    for (iter = storeNew.end() - 1; iter != storeNew.begin() - 1; iter--)
    {
        delete *iter;
    }
    // ת��Ϊ��������
    std::vector<Point>::iterator iter2;
    for (iter2 = route.begin(); iter2 != route.end(); iter2++)
    {
        *iter2 = _gameScene->_tileMap->convertToWorldSpace(*iter2);
    }
}

bool MoveController::canPut(cocos2d::Point position)
{
    Vec2 mapPosition = _gameScene->_tileMap->convertToNodeSpace(position);
    // �Ƿ��ڵ�ͼ��
    if (mapPosition.x < 0 || mapPosition.y < 0)
    {
        return false;
    }
    // �Ƿ��ں���
    if (_gameScene->isCollision(mapPosition))
    {
        return false;
    }
    // �Ƿ��н���ʿ��
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
    // ��û��
    return true;
}