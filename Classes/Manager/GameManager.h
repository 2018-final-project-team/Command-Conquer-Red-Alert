/*
*  @file     GameManager.h
*  @brief    游戏的控制类
*/

#ifndef __GameManager_H_
#define __GameManager_H_

#include "MoveController.h"
#include "../Scene/GameScene.h"
#include "../Data/Building.h"
#include "cocos2d.h""
#include <iostream>
#include <time.h>

class GameScene;

class Manager : public cocos2d::Node
{
private:

    GameScene* _gameScene;
    MoveController* _moveController;

//========CreateController===========
    clock_t _timeToCreateBuilding;               // 单位毫秒
    clock_t _waitToCreateBuilding;
    clock_t _timeToCreateSoldier;
    clock_t _waitToCreateSoldier;
    Tag _buildingTag;
    Tag _soldierTag;
    bool _waitToCreateBuilding;
    bool _canCreateBuilding;
    bool _waitToCreateSoldier;
//===================================

//==========Attack================
    Soldier* _selectedEnemy;              //被选中的士兵或建筑
    Building* _selectedBuilding;

public:

    /**
    * @brief Manager的静态构造函数
    * @return  Manager*
    */
    static Manager* create(GameScene* gameScene);

    /**
    * @brief Manager的setGameScene函数
    * @return  void
    */
    void setGameScene(GameScene* gameScene);

    /**
    * @brief Manager的setMoveController函数
    * @return  void
    */
    void setMoveController(GameScene* gameScene);

    /**
    * @brief Manager的setBool函数
    * @return  void
    */
    void setDatas();

    bool getCanCreateBuilding() { return _canCreateBuilding; }

//=====================CreateController========================
    /**
    * @brief 点击建造建筑的开始
    * @param1 建筑的Tag
    * @param2 开始等待的时间
    * @return  void
    */
    void clickCreateBuildingByTag(Tag building_tag, clock_t start_time);

    /**
    * @brief 点击建造士兵的开始
    * @param1 士兵的Tag
    * @param2 开始等待的时间
    * @return  void
    */
    void clickCreateSoldierByTag(Tag soldier_tag, clock_t start_time);

    /**
    * @brief 建造建筑的等待 需要在update函数里调用
    * @return  void
    */
    void waitCreateBuilding();

    /**
    * @brief 建造士兵的等待 需要在update函数里调用
    * @return  void
    */
    void waitCreateSoldier();

    /**
    * @brief 建造建筑 在GameScene中调用
    * @pamra 造建筑点
    * @return  void
    */
    void createBuilding(cocos2d::Vec2 position);

//============================================================

//====================attack============================
    
    void setEnemy(Soldier* enemy);
    void setBuilding(Building* building);

    /**
    * @brief 攻击 在Update函数里调用
    */
    void attack();

    /**
    * @brief 加钱 在Update函数里调用
    */
    void addMoney();

};

#endif // __GameManeger_H_