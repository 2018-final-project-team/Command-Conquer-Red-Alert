/*
*  @file     MoveController.cpp
*  @brief    各种兵的移动控制
*/

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

void MoveController::moveSoldiers()
{
    // To Do: by czd
    // 本菜鸡实在太菜 QaQ
}