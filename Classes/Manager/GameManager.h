/*
*  @file     GameManager.h
*  @brief    ÓÎÏ·µÄ¿ØÖÆÀà
*/

#ifndef __GameManager_H_
#define __GameManager_H_

#include "MoveController.h"
#include "Scene\GameScene.h"
#include "Data\Building.h"
#include "Data\UnitData.h"
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
    clock_t _timeToCreateBuilding;               // µ¥Î»ºÁÃë
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
    Unit* _selectedEnemy;              //±»Ñ¡ÖÐµÄÊ¿±ø»ò½¨Öþ
    Building* _selectedBuilding;

public:

    /**
    * @brief ManagerµÄ¾²Ì¬¹¹Ôìº¯Êý
    * @return  Manager*
    */
    static Manager* createWithGameScene(GameScene* gameScene);

    /**
    * @brief initWithGameScene
    * @return bool
    */
    bool initWithGameScene(GameScene* gameScene);

    bool getCanCreateBuilding() { return _canCreateBuilding; }

//=====================CreateController========================
    /**
    * @brief µã»÷½¨Ôì½¨ÖþµÄ¿ªÊ¼
    * @param1 ½¨ÖþµÄTag
    * @param2 ¿ªÊ¼µÈ´ýµÄÊ±¼ä
    * @return  void
    */
    void clickCreateBuildingByTag(Tag building_tag, clock_t start_time);

    /**
    * @brief µã»÷½¨ÔìÊ¿±øµÄ¿ªÊ¼
    * @param1 Ê¿±øµÄTag
    * @param2 ¿ªÊ¼µÈ´ýµÄÊ±¼ä
    * @return  void
    */
    void clickCreateSoldierByTag(Tag soldier_tag, clock_t start_time);

    /**
    * @brief ½¨Ôì½¨ÖþµÄµÈ´ý ÐèÒªÔÚupdateº¯ÊýÀïµ÷ÓÃ
    * @return  void
    */
    void waitCreateBuilding();

    /**
    * @brief ½¨ÔìÊ¿±øµÄµÈ´ý ÐèÒªÔÚupdateº¯ÊýÀïµ÷ÓÃ
    * @return  void
    */
    void waitCreateSoldier();

    /**
    * @brief ½¨Ôì½¨Öþ ÔÚGameSceneÖÐµ÷ÓÃ
    * @pamra Ôì½¨Öþµã
    * @return  void
    */
    void createBuilding(cocos2d::Vec2 position);

//============================================================

//====================attack============================
    
    void setEnemy(Unit* enemy);
    void setBuilding(Building* building);

    /**
    * @brief ¹¥»÷ ÔÚUpdateº¯ÊýÀïµ÷ÓÃ
    */
    void attack();

    /**
    * @brief ¼ÓÇ® ÔÚUpdateº¯ÊýÀïµ÷ÓÃ
    */
    void addMoneyUpdate();

};

#endif // __GameManeger_H_