/*
*  @file     MoveController.cpp
*  @brief    ���ֱ����ƶ�����
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
    // ����Ƿ��ϰ�
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

//================ To Do : �˴���ת��bugʮ������,�κα�����������,��ʱ�벻���õķ��� ==================
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
//================ To Do : �˴���ת��bugʮ������,�κα�����������,��ʱ�벻���õķ��� ==================
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
            //// �����ײ
            if (_gameScene->isCollision(move + nowPosition))
            {
                return;
            }
            

            _isFirstMove = false;
        }
    }
}

//����Ѱ·�Ľڵ�
//whereX,whereY,��ǰʿ��λ�ã��Ե�ͼ����Ϊԭ��
//cost ����
//father ���ڵ㣬�����ҵ�·�󷵻�

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
//Ѱ·�㷨����ɲ���
//�жϵ�ǰ�ҵ��ĵغ�Ŀ�ĵؼ��Ƿ����ϰ�
//���û���ϰ�������Ѱ·
//ǰ���·���ҵ���·�ߣ������·ֱ����ֱ��
//���뵱ǰλ�ã�Ŀ�ĵأ���ͼ���꣩
bool MoveController::is_find(Vec2 position,Vec2 destination) {
	Point direction = destination - position;
	//����֮����ȡ15���㣬�����û�ϰ�����û�ϰ�
	for (int i = 1; i < 15; i++) {
		if (!_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(position+i*direction/20))) {
			return false;
		}
	}
	return true;
}
/**
Ѱ·�㷨����
����A*�㷨
* @brief ����Ҫ�ƶ���ʿ����һ��Vector<Point*> route,��װ�ҵ���·
Ϊ��Ч�ʿ��ǣ����������ڵ��distance=240,������ͨ�������ٶȵõ�ÿ���Լ�ÿ֡��λ��
by czd
* @return  void
*/
void MoveController::findRroute(Unit *&soldier, Vector<Point> &route) {
	std::vector<node*> vn;
	Vec2 screenNowPosition = soldier->getPosition();
	Vec2 screenDestination = soldier->getDestination();
	//ת��Ϊ�Ե�ͼ���½�Ϊԭ�������
	Vec2 nowPosition = _gameScene->_tileMap->convertToNodeSpace(screenNowPosition);
	Vec2 nowDestination= _gameScene->_tileMap->convertToNodeSpace(screenDestination);
	//����Ժ��������
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
	//8������
	//�������ң��������µ�
	float directX[8] = { 0,0,-1 * distance ,distance,0.7*distance ,0.7* distance ,-0.7* distance ,-0.7* distance };
	float directY[8] = { distance,-1*distance,0 ,0,0.7* distance ,-0.7* distance ,0.7* distance ,-0.7* distance };
	int is_not_find = 1;
	//������������ϰ�
	if (is_find(nowPosition,nowDestination)) {
		is_not_find = 0;
	}
	while (is_not_find) {
		node *cur = open.front();
		//close.push(cur);
		open.pop();
		//�о�8������
		for (int i = 0; i < 8; i++) {
			//��λ���Ƿ����ϰ�
			if (_gameScene->isCollision(_gameScene->_tileMap->convertToWorldSpace(Point(cur->whereX+directX[i], cur->whereY+directY[i])))) {
				//��һ��ɸѡ
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
	//�õغ�Ŀ�ĵؿ�ֱ��
	Point nowPlace{ 0,0 };
	nowPlace.x = static_cast<float>(myend->whereX);
	nowPlace.y = static_cast<float>(myend->whereY);
	Point direction = nowDestination - nowPlace;
	direction.normalize();
	//���ҵ��ĵص�������
	while (myend!= NULL) {
		//route.pushBack(Point(myend->whereX, myend->whereY));
		myend = myend->father;
	}
	//reverse�󣬾��ǳ����ص��ҵ���
	route.reverse();
	//�ٰѺ����·����route
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