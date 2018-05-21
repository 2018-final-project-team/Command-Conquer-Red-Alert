#ifndef __Welcome_SCENE_H__
#define __Welcome_SCENE_H__

#include "cocos2d.h"

class WelcomeScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void enterGameScene(Ref *pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(WelcomeScene);

	/**
	@brief  �Ӿ���ͼ���л�ȡ������صľ���֡��Vector
	@param  sheetPlistName ����1 ����ͼ����Ӧplist���ļ����ƣ�ע��Ҫ����·��
	@param  format         ����2 ����ͼ�����붯�����ͼƬ���Ƶĸ�ʽ��ע��Ҫ����·��
	@param  count          ����3 ���ͼƬ������
	@return Vector<SpriteFrame*>
	*/
	cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
};

#endif // __Welcome_SCENE_H__