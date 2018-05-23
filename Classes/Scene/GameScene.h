#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"

typedef enum
{
    BASE_TAG,
    POWER_PLANT_TAG,
    MINE_TAG,
    BARRACKS_TAG,
    CAR_FACTORY_TAG,
    INFANTRY_TAG,
    DOG_TAG,
    TANK_TAG
}Tag;

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuBackCallback(Ref *pSender);
};

#endif // __Welcome_SCENE_H__