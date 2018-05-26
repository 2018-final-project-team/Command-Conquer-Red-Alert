/*
*  @file     MoveController.h
*  @brief    ¸÷ÖÖ±øµÄÒÆ¶¯Àà
*/

#ifndef __MoveController_H_
#define __MoveController_H_

#include "cocos2d.h""
#include <iostream>
#include "../Scene/GameScene.h"
#include "../Data/Building.h"
#include "../Data/UnitData.h"

class GameScene;

/*
*@brief ¸÷ÖÖ±øµÄÒÆ¶¯¿ØÖÆÀà
*¿ØÖÆ±øµÄÒÆ¶¯
*/
//=========================To Do: ÅÅ¶ÓÐÍ ºÍ Ã»ÓÐbugµÄ×ªÏò==========================

class MoveController : public cocos2d::Node
{

protected:

    GameScene* _gameScene;                                      // game ³¡¾°Àà
    cocos2d::Vector<Unit*>* _selectedSoldiers;     // selected soldiers
    bool _isFirstMove;                               //ÓÃÀ´¼õÉÙÕÏ°­ÅÐ¶Ï´ÎÊý

public:

    /**
    * @brief MoveControllerµÄ¾²Ì¬¹¹Ôìº¯Êý
    * @return  MoveController*
    */
    static MoveController* createWithGameScene(GameScene* gameScene);

    /**
    * @brief initWithGameScene
    * @return bool
    */
    bool initWithGameScene(GameScene* gameScene);

    /**
    * @brief ÓÃÊó±êÑ¡ÔñÊ¿±øÌ¹¿Ë
    * @param Êó±ê°´ÏÂµÄµã
    * @param Êó±êËÉ¿ªµÄµã
    * @return  void
    */
    void selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint);

    /**
    * @brief ÓÃ¿ì½Ý¼ü(´«µÝÃû×Ö)Ñ¡ÔñÊ¿±øÌ¹¿Ë
    # @param ±øÖÖµÄÃû×Ö
    * @return  void
    */
    void selectSoldiersWithName(const std::string& name);

    /**
    * @brief ÉèÖÃÊ¿±øÌ¹¿ËÄ¿µÄµØ(²»ÄÜÊÇÕÏ°­)
    * @param Ä¿µÄµØµã
    * @return  void
    */
    void setDestination(cocos2d::Vec2 position);

    /**
    * @brief ÅÐ¶ÏÏòÇ°5¸öÍßÆ¬ÄÚÊÇ·ñ»áÅö×²
    * @param µ±Ç°Î»ÖÃ
    * @param ·½Ïò
    * @return  int
    */
    bool willHitInFiveTiles(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction);

    /**
    * @brief Óöµ½ÕÏ°­Îï×ªÏò
    * @param µ±Ç°Î»ÖÃ
    * @param ·½Ïò
    * @return  ×ªÏòºó·½Ïò
    */
    cocos2d::Vec2 changeDirection(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction);

    /**
    * @brief ÒÆ¶¯Ê¿±øÌ¹¿Ë ÔÚUpdateº¯ÊýÀïµ÷ÓÃ
    * @return  void
    */
    void moveSoldiers();

};

#endif // __MoveController_H_