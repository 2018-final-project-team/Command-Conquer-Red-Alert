/*
*  @file     GameScene.h
*  @brief    游戏场景类
*  @author   王亮 蔡震栋
*/

#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{

	cocos2d::TMXTiledMap* _tileMap;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void update(float time);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuBackCallback(Ref *pSender);

	

private:
	Point _cursorPosition{ 0,0 };
	void scrollMap();
};

#endif // __Welcome_SCENE_H__