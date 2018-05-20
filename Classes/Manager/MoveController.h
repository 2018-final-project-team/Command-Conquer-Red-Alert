/*
*  @file     MoveController.h
*  @brief    各种兵的移动类
*  @author   何立仁
*  @email    hlr12138@outlook.com
*/

#ifndef __MoveController_H_
#define __MoveController_H_

#include <cocos2d.h>
#include <iostream>

class Game;

/*
*@brief 各种兵的移动控制类
*控制兵的移动
*/
//=========================To Do: 排队型 和 没有bug的转向==========================

class MoveController : public cocos2d::Node
{

protected:

    Game* _game;                                      // game 场景类
    cocos2d::Vector<Soldier*>* _selectedSoldiers;     // selected soldiers
    bool _isFirstMove;                               //用来减少障碍判断次数

public:

    /**
    * @brief MoveController的静态构造函数
    * @param game 参数1
    * @return  MoveController*
    */
    static MoveController* create(Game* game);

    /**
    * @brief MoveController的setGameScene函数
    * @param game 参数1
    * @return  void
    */
    void setGameScene(Game* game);

    /**
    * @brief 用鼠标选择士兵坦克
    * @param 鼠标按下的点
    * @param 鼠标松开的点
    * @return  void
    */
    void selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint);

    /**
    * @brief 用快捷键(传递名字)选择士兵坦克
    # @param 兵种的名字
    * @return  void
    */
    void selectSoldiersWithName(const std::string& name);

    /**
    * @brief 设置士兵坦克目的地(不能是障碍)
    * @param 目的地点
    * @return  void
    */
    void setDestination(cocos2d::Vec2 position);

    /**
    * @brief 判断向前5个瓦片内是否会碰撞
    * @param 当前位置
    * @param 方向
    * @return  int
    */
    bool willHitInFiveTiles(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction);

    /**
    * @brief 遇到障碍物转向
    * @param 当前位置
    * @param 方向
    * @return  转向后方向
    */
    cocos2d::Vec2 changeDirection(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction);

    /**
    * @brief 移动士兵坦克
    * @return  void
    */
    void moveSoldiers();

};

#endif // __MoveController_H_