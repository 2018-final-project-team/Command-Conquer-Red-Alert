/*
*  @file     MoveController.cpp
*  @brief    各种兵的移动控制
*/
#pragma once
#include "MoveController.h"
#include "../Data/Building.h"
#include "../Data/UnitData.h"
#include <time.h>

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
    _isFirstMove = true;

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
    if (_gameScene->isCollision(position))
    {
        return;
    }

    for (auto& soldier : *_selectedSoldiers)
    {
        soldier->setDestination(position);
        soldier->setGetDestination(false);
    }

    _isFirstMove = true;
}

bool MoveController::willHitInFiveTiles(Vec2 nowPosition, Vec2 direction)
{
    float tile_size = _gameScene->getTileSize();
    if (_isFirstMove)
    {
        for (int i = 0; i < 5; ++i)
        {
            nowPosition += direction * tile_size;
            if (_gameScene->isCollision(nowPosition))
            {
                return true;
            }
        }
    }
    else if (_gameScene->isCollision(nowPosition + direction * tile_size * 5))
    {
        return true;
    }
    
    return false;
}

Vec2 MoveController::changeDirection(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction)
{
    float tile_size = _gameScene->getTileSize();
    bool store_is_first_move = _isFirstMove;
    _isFirstMove = true;

//================ To Do : 此处的转向bug十分严重,奈何本人能力有限,暂时想不出好的方法 ==================
    Vec2 up(0, 1);
    Vec2 down(0, -1);
    Vec2 right(1, 0);
    Vec2 left(-1, 0);
    if (direction.x > 0)
    {
        if (direction.y > 0)                                         //right up
        {
            if (!willHitInFiveTiles(nowPosition, up))       // up
                return up;
            else if (!willHitInFiveTiles(nowPosition, right))   // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, left))  //left
                return left;
            else if (!willHitInFiveTiles(nowPosition, down))   // down
                return down;
            else
                return -direction;
        }
        else if (direction.y < 0)                                   // right down
        {
            if (!willHitInFiveTiles(nowPosition, down))     // down
                return down;
            else if (!willHitInFiveTiles(nowPosition, right)) // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, left)) // left
                return left;
            else if (!willHitInFiveTiles(nowPosition, up)) // up
                return up;
            else
                return -direction;
        }
        else                                                         // right
        {
            if (!willHitInFiveTiles(nowPosition, up))        // up
                return up;
            else if (!willHitInFiveTiles(nowPosition, down))  // down
                return down;
            else
                return -direction;
        }
    }
    else if (direction.x < 0)                                            
    {
        if (direction.y > 0)                                               // left up
        {
            if (!willHitInFiveTiles(nowPosition, up))     // up
                return up;
            else if (!willHitInFiveTiles(nowPosition, left)) // left
                return left;
            else if (!willHitInFiveTiles(nowPosition, right)) // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, down)) // down
                return down;
            else
                return -direction;
        }
        else if (direction.y < 0)                                         //left down
        {
            if (!willHitInFiveTiles(nowPosition, down))     // down
                return down;
            else if (!willHitInFiveTiles(nowPosition, left)) // left
                return left;
            else if (!willHitInFiveTiles(nowPosition, right)) // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, up)) // up
                return up;
            else
                return -direction;
        }
        else                                                            //left
        {
            if (!willHitInFiveTiles(nowPosition, down))     // down
                return down;
            else if (!willHitInFiveTiles(nowPosition, up)) // up
                return up;
            else
                return -direction;
        }
    }
    else                                               // up or down                          
    {
        if (!willHitInFiveTiles(nowPosition, right)) // right
            return right;
        else if (!willHitInFiveTiles(nowPosition, left)) // left
            return left;
        else
            return -direction;
    }

    _isFirstMove = store_is_first_move;
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
//================ To Do : 此处的转向bug十分严重,奈何本人能力有限,暂时想不出好的方法 ==================
            if (willHitInFiveTiles(nowPosition, direction))
            {
                if (soldier->getSecondDirection() != Vec2::ZERO)
                {
                    soldier->setSecondDirection(changeDirection(nowPosition, direction));
                }
                direction = soldier->getSecondDirection();
            }
            else
            {
                soldier->setSecondDirection(Vec2::ZERO);
            }

            Vec2 move = soldier->getUnitSpeed() * interval * direction;
            if (move.length() > distance)
            {
                soldier->moveTo(destination, 0.01);
                soldier->setGetDestination(true);
                _isFirstMove = true;
                return;
            }
			else {
				soldier->moveTo(move + nowPosition, 0.01);
			}
            //// 检测碰撞
            if (_gameScene->isCollision(move + nowPosition))
            {
                return;
            }
            

            _isFirstMove = false;
        }
    }
}

//用于寻路的节点
//whereX,whereY,当前士兵位置，以地图左下为原点
//cost 花费
//father 父节点，用于找到路后返回

struct node {
	int whereX;
	int whereY;
	int cost;
	node* father;
	node(int wx, int wy, int c, node* f) {
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
//传入当前位置，目的地（地图坐标）
bool MoveController::is_find(Vec2 position,Vec2 destination) {
	Point direction = destination - position;
	//两点之间任取15个点，如果都没障碍，即没障碍
	for (int i = 1; i < 15; i++) {
		if (!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(position+i*direction/20))) {
			return false;
		}
	}
	return true;
}
/**
寻路算法主体
类似A*算法
* @brief 传入要移动的士兵和一个Vector<Point*> route,来装找到的路
为了效率考虑，相邻两个节点的distance=240,，可以通过除以速度得到每秒以及每帧的位置
by czd
* @return  void
*/
void MoveController::findRroute(Unit *&soldier, Vector<Point> &route) {
	std::vector<node*> vn;
	Vec2 screenNowPosition = soldier->getPosition();
	Vec2 screenDestination = soldier->getDestination();
	//转化为以地图左下角为原点的坐标
	Vec2 nowPosition = _gameScene->_tileMap->convertToNodeSpace(screenNowPosition);
	Vec2 nowDestination= _gameScene->_tileMap->convertToNodeSpace(screenDestination);
	//这个以后可能有用
	//struct cmp {
	//	bool operator()(node *a, node *b) {
	//		return a->cost >= b->cost;
	//	}
	//};
	std::queue<node*> open;
	//std::priority_queue<node*, std::vector<node*>, cmp> close;
	node *head = new node(nowPosition.x, nowPosition.y, 0,NULL);
	node *myend=head;
	open.push(head);
	int distance = 240;
	//8个方向
	//上下左右，左上左下等
	float directX[8] = { 0,0,-1 * distance ,distance,0.7*distance ,0.7* distance ,-0.7* distance ,-0.7* distance };
	float directY[8] = { distance,-1*distance,0 ,0,0.7* distance ,-0.7* distance ,0.7* distance ,-0.7* distance };
	int is_not_find = 1;
	//这两个点间无障碍
	if (is_find(nowPosition,nowDestination)) {
		is_not_find = 0;
	}
	while (is_not_find) {
		node *cur = open.front();
		//close.push(cur);
		open.pop();
		//列举8个方向
		for (int i = 0; i < 8; i++) {
			//绑定位置是否有障碍
			if (_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(Point(cur->whereX+directX[i], cur->whereY+directY[i])))) {
				//进一步筛选
				if (Vec2(nowDestination.x - (cur->whereX), nowDestination.y - (cur->whereY)).length() > Vec2(nowDestination.x - (cur->whereX) - directX[i], nowDestination.y - (cur->whereY) - directY[i]).length() - 0.7*distance) {
					node *n = new node(cur->whereX + directX[i], cur->whereY + directY[i], distance, cur);
					vn.push_back(n);
					if (is_find(Point(n->whereX,n->whereY),nowDestination)) {
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
	while (myend!= NULL) {
		//route.pushBack(Point(myend->whereX, myend->whereY));
		myend = myend->father;
	}
	//reverse后，就是出发地到找到地
	route.reverse();
	//再把后面的路加入route
	while ((nowDestination - nowPlace).length() > distance) {
		//route.pushBack(nowPlace + distance*direction);
		nowPlace += distance*direction;
	}
	//route.pushBack(nowDestination);
	std::vector<node*>:: iterator it;
	it = vn.begin();
	while (it != vn.end()) {
		delete *it;
		it++;
	}

}