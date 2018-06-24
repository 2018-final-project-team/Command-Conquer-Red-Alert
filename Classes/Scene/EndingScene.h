/*
*  @file     EndingScene.cpp
*  @brief    ��������
*  @author   ������
*/
#ifndef _ENDINGSCENE_H_
#define _ENDINGSCENE_H_

#include "cocos2d.h"

class EndingScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(cocos2d::Ref* pSender);

	// ������ת������������menu�ص�����
	void menuWelcomeCallback(cocos2d::Ref *pSender);
	
	bool isWinner();

	bool isLoser();

	CREATE_FUNC(EndingScene);
};


#endif // !_ENDINGSCENE_H_

