/*
*  @file     MoveController.cpp
*  @brief    各种兵的移动控制
*  @author   何立仁
*  @email    hlr12138@outlook.com
*/

#include "MoveController.h"
#include "../Scene/GameScene.h"
#include <time.h>

USING_NS_CC;

MoveController* MoveController::create(Game* game)
{
    MoveController* move_controller = new MoveController();
    if (move_controller)
    {
        move_controller->setGameScene(game);
        move_controller->_selectedSoldiers = game->getSelectedSoldiers();
        move_controller->autorelease();
        return move_controller;
    }

    CC_SAFE_DELETE(move_controller);
    return nullptr;
}

void MoveController::setGameScene(Game* game)
{
    _game = game;
}

void MoveController::selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint)
{
    _selectedSoldiers.clean();
    float rect_width = fabs(mouseUpPoint.x - mouseDownPoint.x);
    float rect_height = fabs(mouseUpPoint.y - mouseDownPoint.y);
    Rect rect(MIN(mouseDownPoint.x, mouseUpPoint.x), MIN(mouseDownPoint.y, mouseUpPoint.y), rect_width, rect_height);
    for (auto& soldier : _game->getSoldiers())
    {
        if (rect.containsPoint(soldier->getPosition()))
        {
            soldier->setIsSelected(true);
            _selectedSoldiers.pushBack(soldier);
        }
    }
}

void MoveController::selectSoldiersWithName(const std::string& name)
{
    _selectedSoldiers.clean();
    for (auto& soldier : _game->getSoldiers())
    {
        if (soldier->getName() == name)
        {
            soldier->setIsSelected(true);
            _selectedSoldiers.pushBack(soldier);
        }
    }
}

void MoveController::setDestination(cocos2d::Vec2 position)
{
    // 检测是否障碍
    if (_game->isCollision(position))
    {
        return;
    }

    for (auto& soldier : _selectedSoldiers)
    {
        soldier->setDestination(position);
        soldier->setGetDestination(false);
    }

    _isFirstMove = true;
}

bool MoveController::willHitInFiveTiles(Vec2 nowPosition, Vec2 direction)
{
    float tile_size = _game->getTileMap()->getTileSize().width;
    if (_isFirstMove)
    {
        for (int i = 0; i < 5; ++i)
        {
            nowPosition += direction * tile_size;
            if (_game->isCollision(nowPosition))
            {
                return true;
            }
        }
    }
    else if (_game->isCollision(nowPosition + direction * tile_size * 5))
    {
        return true;
    }
    
    return false;
}

Vec2 MoveController::changeDirection(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction)
{
    float tile_size = _game->getTileMap()->getTileSize().width;
    bool store_is_first_move = _isFirstMove;
    _isFirstMove = true;

//================ To Do : 此处的转向bug十分严重,奈何本人能力有限,暂时想不出好的方法 ==================
    if (_game->getMapType() == 0)
    {
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
    }
    else if (_game->getMapType() == 45)
    {
        Vec2 up(1, 1);
        Vec2 down(-1, -1);
        Vec2 right(1, -1);
        Vec2 left(-1, 1);
        if (direction.x > direction.y)
        {
            if (direction.y < -direction.x)                                         //right up
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
            else if (direction.y > -direction.y)                                   // right down
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
        else if (direction.x < direction.y)
        {
            if (direction.y < -direction.x)                                               // left up
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
            else if (direction.y > -direction.x)                                         //left down
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
    }
}

void MoveController::moveSoldiers()
{
    static clock_t preT = clock();
    clock_t nowT = clock();
    float interval = (nowT - preT) / 1000;
    preT = nowT;
    for (auto& soldier : _game->getSoldiers())
    {
        if (!soldier->getIfGetDestination())
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

            Vec2 move = soldier->getSpeed() * interval * direction;
            if (move.length > distance)
            {
                soldier->moveTo(destination);
                soldier->setGetDestination(true);
                return;
            }
            
            // 检测碰撞
            if (_game->isCollision(move + nowPosition))
            {
                return;
            }
            soldier->moveTo(move + nowPosition);
        }
    }
}