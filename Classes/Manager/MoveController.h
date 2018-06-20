/*
*  @file     MoveController.h
*  @brief    controll soldiers' movement
*/

#ifndef __MoveController_H_
#define __MoveController_H_

#include "cocos2d.h"
#include <iostream>
#include <vector>
#include "Scene/GameScene.h"

class GameScene;

class MoveController : public cocos2d::Node
{

protected:

    GameScene* _gameScene;                             // game scene
    cocos2d::Vector< Unit* > *_selectedSoldiers;     // selected soldiers

public:

    /**
    * @brief MoveController static create function
    * @return  MoveController*
    */
    static MoveController* createWithGameScene(GameScene* gameScene);

    /**
    * @brief initWithGameScene
    * @return bool
    */
    bool initWithGameScene(GameScene* gameScene);

    /**
    * @brief select soldiers with mouse
    * @param the position of mouse down
    * @param the position of mouse up
    * @return  void
    */
    void selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint);

    /**
    * @brief select soldiers with Tag
    # @param tag of soldier
    * @return  void
    */
    void selectSoldiersWithTag(Tag tag);

    /**
    * @brief set soldiers' destination
    * @param destination
    * @return  void
    */
    void setDestination(cocos2d::Vec2 position);

    /**
    * @brief call in Update function
    * @return  void
    */
    void moveSoldiers();

    void findRroute(Unit *soldier, std::vector<cocos2d::Point> &route);
    
    bool is_find(cocos2d::Vec2 position, cocos2d::Vec2 destination);

    /*
    * @brief can put slidier at this position
    */
    bool canPut(cocos2d::Point position);

};

#endif // __MoveController_H_
