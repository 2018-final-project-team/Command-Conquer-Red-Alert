/*
*  @file     MoveController.h
*  @brief    各种兵的移动类
*/

#ifndef __MoveController_H_
#define __MoveController_H_

#include "cocos2d.h""
#include <iostream>
#include <vector>
#include "Scene\GameScene.h"

class GameScene;

/*
*@brief 各种兵的移动控制类
*控制兵的移动
*/
//=========================To Do: 排队型 和 寻路==========================

class MoveController : public cocos2d::Node
{

protected:

    GameScene* _gameScene;                             // game 场景类
    cocos2d::Vector< Unit* > *_selectedSoldiers;     // selected soldiers

public:

    /**
    * @brief MoveController的静态构造函数
    * @return  MoveController*
    */
    static MoveController* createWithGameScene(GameScene* gameScene);

    /**
    * @brief initWithGameScene
    * @return bool
    */
    bool initWithGameScene(GameScene* gameScene);

    /**
    * @brief 用鼠标选择士兵坦克
    * @param 鼠标按下的点
    * @param 鼠标松开的点
    * @return  void
    */
    void selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint);

    /**
    * @brief 用快捷键(传递Tag)选择士兵坦克
    # @param 兵种的名字
    * @return  void
    */
    void selectSoldiersWithTag(Tag tag);

    /**
    * @brief 设置士兵坦克目的地(不能是障碍)
    * @param 目的地点
    * @return  void
    */
    void setDestination(cocos2d::Vec2 position);

    /**
    * @brief 移动士兵坦克 在Update函数里调用
    * @return  void
    */
    void moveSoldiers();

    void findRroute(Unit *soldier, std::vector<Point> &route);
    /**
    * @brief 传入起点和目的地(屏幕）和一个Vector<Point> route,来装找到的路
    如，第一帧，把坦克原坐标加上route[1],在刷新
    * @return  void
    */
    bool is_find(Vec2 position, Vec2 destination);

};

#endif // __MoveController_H_
