/*
*  @file     LoadingScene.h
*  @brief    加载场景类，显示的第一个场景，完成资源预加载并显示加载进度
*  @author   王亮
*/

#ifndef __SCENE_LOADINGSCENE_H__
#define __SCENE_LOADINGSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LoadingScene : public cocos2d::Layer
{
private:
	cocos2d::ui::LoadingBar* loadingBar;

private:

	/**
	@brief 资源全部加载结束后供scheduleOnce调用的函数，延时dt跳转至登录场景
	@param float dt 时间间隔，单位为秒
	*/
	void endLoading(float dt);

	/**
	@brief 加载过程，进行资源的加载和进度条的更新
	*/
	void loading();

	/**
	@brief 加载各类资源
	*/
	void loadSound();
	void loadImage();
	void loadSpriteFrame();
	void loadAnimation();


	void initBackground(cocos2d::Vec2 origin, cocos2d::Size visibleSize);
	cocos2d::ui::LoadingBar* createLoadingBar();
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);
};

#endif // __SCENE_LOADINGSCENE_H__