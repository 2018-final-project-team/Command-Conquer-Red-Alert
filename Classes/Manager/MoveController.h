/*
*  @file     MoveController.h
*  @brief    ���ֱ����ƶ���
*/

#ifndef __MoveController_H_
#define __MoveController_H_

#include "cocos2d.h""
#include <iostream>
#include <vector>
#include "Scene\GameScene.h"

class GameScene;

/*
*@brief ���ֱ����ƶ�������
*���Ʊ����ƶ�
*/
//=========================To Do: �Ŷ��� �� Ѱ·==========================

class MoveController : public cocos2d::Node
{

protected:

    GameScene* _gameScene;                             // game ������
    cocos2d::Vector< Unit* > *_selectedSoldiers;     // selected soldiers

public:

    /**
    * @brief MoveController�ľ�̬���캯��
    * @return  MoveController*
    */
    static MoveController* createWithGameScene(GameScene* gameScene);

    /**
    * @brief initWithGameScene
    * @return bool
    */
    bool initWithGameScene(GameScene* gameScene);

    /**
    * @brief �����ѡ��ʿ��̹��
    * @param ��갴�µĵ�
    * @param ����ɿ��ĵ�
    * @return  void
    */
    void selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint);

    /**
    * @brief �ÿ�ݼ�(����Tag)ѡ��ʿ��̹��
    # @param ���ֵ�����
    * @return  void
    */
    void selectSoldiersWithTag(Tag tag);

    /**
    * @brief ����ʿ��̹��Ŀ�ĵ�(�������ϰ�)
    * @param Ŀ�ĵص�
    * @return  void
    */
    void setDestination(cocos2d::Vec2 position);

    /**
    * @brief �ƶ�ʿ��̹�� ��Update���������
    * @return  void
    */
    void moveSoldiers();

    void findRroute(Unit *soldier, std::vector<Point> &route);
    /**
    * @brief ��������Ŀ�ĵ�(��Ļ����һ��Vector<Point> route,��װ�ҵ���·
    �磬��һ֡����̹��ԭ�������route[1],��ˢ��
    * @return  void
    */
    bool is_find(Vec2 position, Vec2 destination);

};

#endif // __MoveController_H_
