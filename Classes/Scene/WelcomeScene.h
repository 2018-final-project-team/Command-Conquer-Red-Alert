/*
*  @file     WelcomeScene.h
*  @brief    欢迎（主）场景类，可通过此场景进入游戏、设置、帮助场景，可退出游戏
*  @author   王亮
*/

#ifndef __Welcome_SCENE_H__
#define __Welcome_SCENE_H__

#include "cocos2d.h"

class WelcomeScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	// 用于跳转到其他场景的menu回调函数
	void menuPlayCallback(Ref *pSender);
	void menuSettingsCallback(cocos2d::Ref * pSender);
	void menuHelpCallback(cocos2d::Ref * pSender);

	CREATE_FUNC(WelcomeScene);

};

#endif // __Welcome_SCENE_H__