/*
*  @file     WelcomeScene.h
*  @brief    ��ӭ�����������࣬��ͨ���˳���������Ϸ�����á��������������˳���Ϸ
*  @author   ����
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

	// ������ת������������menu�ص�����
	void menuPlayCallback(Ref *pSender);
	void menuSettingsCallback(cocos2d::Ref * pSender);
	void menuHelpCallback(cocos2d::Ref * pSender);

	CREATE_FUNC(WelcomeScene);

	/**
	@brief  �Ӿ���ͼ�������л�ȡ������صľ���֡��Vector
	@param  format         ����1 ����ͼ�����붯�����ͼƬ���Ƶĸ�ʽ��ע��Ҫ����·��
	@param  count          ����2 ���ͼƬ������
	@return Vector<SpriteFrame*>
	*/
	cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
};

#endif // __Welcome_SCENE_H__