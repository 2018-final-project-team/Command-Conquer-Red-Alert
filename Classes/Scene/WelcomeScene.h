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
	@brief  从精灵图集中获取动画相关的精灵帧的Vector
	@param  sheetPlistName 变量1 精灵图集对应plist的文件名称，注意要包含路径
	@param  format         变量2 精灵图集中与动画相关图片名称的格式，注意要包含路径
	@param  count          变量3 相关图片的数量
	@return Vector<SpriteFrame*>
	*/
	cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
};

#endif // __Welcome_SCENE_H__