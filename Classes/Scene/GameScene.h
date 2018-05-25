/*
*  @file     GameScene.h
*  @brief    游戏场景类
*  @author   王亮
*/

#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuBackCallback(Ref *pSender);
};

#endif // __Welcome_SCENE_H__