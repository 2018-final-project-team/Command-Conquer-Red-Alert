#include "Util/GameAnimation.h"

USING_NS_CC;

GameAnimation* GameAnimation::instance;

GameAnimation* GameAnimation::getInstance()
{
	if (instance == NULL)
		instance = new GameAnimation();
	return instance;
}

void GameAnimation::init()
{
	initInfantry();
	initDog();
	initTank();
}

Animation* GameAnimation::createAnimation(const char* prefixName, int start, int end, float delay)
{
	Vector<SpriteFrame*> animFrames;

	for (int i = start; i <= end; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(prefixName, i)->getCString());
		if (frame != nullptr)
			animFrames.pushBack(frame);
	}
	return Animation::createWithSpriteFrames(animFrames, delay);
}

Vector<SpriteFrame*> GameAnimation::getAnimation(const char* format, int count)
{
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
	}
	return animFrames;
}


void GameAnimation::initInfantry()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("GameItem/Unit/Unit1-forwar%02d.png", 1, 12, 0.2f), "infantry_forward");
	AnimationCache::getInstance()->addAnimation(createAnimation("GameItem/Unit/Unit1-back%02d.png", 1, 12, 0.2f), "infantry_back");
	AnimationCache::getInstance()->addAnimation(createAnimation("GameItem/Unit/Unit1-right%02d.png", 1, 12, 0.2f), "infantry_right");
	AnimationCache::getInstance()->addAnimation(createAnimation("GameItem/Unit/Unit1-attack%02d.png", 1, 10, 0.2f), "infantry_attack");
	AnimationCache::getInstance()->addAnimation(createAnimation("GameItem/Unit/Unit1-die%02d.png", 1, 6, 0.2f), "infantry_die");



}

void GameAnimation::initDog()
{

}

void GameAnimation::initTank()
{

}