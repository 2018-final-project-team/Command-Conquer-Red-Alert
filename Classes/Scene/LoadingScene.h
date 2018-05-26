/*
*  @file     LoadingScene.h
*  @brief    ���س����࣬��ʾ�ĵ�һ�������������ԴԤ���ز���ʾ���ؽ���
*  @author   ����
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
	@brief ��Դȫ�����ؽ�����scheduleOnce���õĺ�������ʱdt��ת����¼����
	@param float dt ʱ��������λΪ��
	*/
    void endLoading(float dt);

	/**
	@brief ���ع��̣�������Դ�ļ��غͽ������ĸ���
	*/
	void loading();

	/**
	@brief ���ظ�����Դ
	*/
    void loadSound();
    void loadImage();
    void loadSpriteFrame();


	void initBackground(cocos2d::Vec2 origin, cocos2d::Size visibleSize);
    cocos2d::ui::LoadingBar* createLoadingBar();
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);
};

#endif // __SCENE_LOADINGSCENE_H__