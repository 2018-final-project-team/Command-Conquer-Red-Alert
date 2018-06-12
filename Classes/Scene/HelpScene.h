/*
*  @file     HelpScene.h
*  @brief    帮助场景类，显示游戏玩法和操作方法
*  @author   王亮
*/

#ifndef __SCENE_HELPSCENE_H__
#define __SCENE_HELPSCENE_H__

#include "cocos2d.h"
#include"ui/UIButton.h"

class HelpScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(HelpScene);

	void menuBackCallback(Ref *pSender);
};

#endif // __HELP_SCENE_H__