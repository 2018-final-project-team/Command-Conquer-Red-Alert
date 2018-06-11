#ifndef __UTIL_GAMEANIMATION_H__
#define __UTIL_GAMEANIMATION_H__

#include "cocos2d.h"

class GameAnimation
{	
private:
	static GameAnimation * instance;

public:
	static GameAnimation* getInstance();

	/**
	@brief  加载动画，在LoadingScene中调用
	*/
	void init();

	void initInfantry();
	void initDog();
	void initTank();

	cocos2d::Animation* createAnimation(const char* prefixName, int start, int end, float delay);

	/**
	@brief  从精灵图集缓存中获取动画相关的精灵帧的Vector
	@param  format         变量1 精灵图集中与动画相关图片名称的格式，注意要包含路径
	@param  count          变量2 相关图片的数量
	@return Vector<SpriteFrame*>
	*/
	static cocos2d::Vector<cocos2d::SpriteFrame*> getAnimation(const char* format, int count);
};

#endif