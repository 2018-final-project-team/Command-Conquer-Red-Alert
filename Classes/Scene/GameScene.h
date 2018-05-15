#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"

class Game : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	//void menuCloseCallback(cocos2d::Ref* pSender);

	void enterWelcomeScene(Ref *pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);
};

#endif // __Welcome_SCENE_H__