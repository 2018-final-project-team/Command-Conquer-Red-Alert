/*
*  @file     EndingScene.cpp
*  @brief    结束场景
*  @author   王星洲
*/
#ifndef _ENDINGSCENE_H_
#define _ENDINGSCENE_H_

#include "cocos2d.h"

class EndingScene : public cocos2d::Layer
{
private:
	bool _isWin;

public:
	static cocos2d::Scene* createScene(bool isWin);

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	// 用于跳转到其他场景的menu回调函数
	void menuWelcomeCallback(cocos2d::Ref *pSender);
	
	bool isWinner();

	CREATE_FUNC(EndingScene);
};


#endif // !_ENDINGSCENE_H_

