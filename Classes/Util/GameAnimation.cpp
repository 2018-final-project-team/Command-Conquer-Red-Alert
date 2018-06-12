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
	initBaseCar();
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
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-forward (%d).png", 1, 12, 0.2f), "infantry_forward");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-back (%d).png", 1, 12, 0.2f), "infantry_back");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-left (%d).png", 1, 12, 0.2f), "infantry_left");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-right (%d).png", 1, 12, 0.2f), "infantry_right");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-attackLeft (%d).png", 1, 10, 0.2f), "infantry_attackLeft");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-attackRight (%d).png", 1, 10, 0.2f), "infantry_attackRight");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit1-die (%d).png", 1, 6, 0.2f), "infantry_die");
}

void GameAnimation::initDog()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-forward (%d).png", 1, 5, 0.1f), "dog_forward");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-back (%d).png", 1, 5, 0.1f), "dog_back");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-left (%d).png", 1, 9, 0.2f), "dog_left");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-right (%d).png", 1, 9, 0.2f), "dog_right");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-attackLeft (%d).png", 1, 10, 0.2f), "dog_attackLeft");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-attackRight (%d).png", 1, 10, 0.2f), "dog_attackRight");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit2-die (%d).png", 1, 9, 0.2f), "dog_die");
}

void GameAnimation::initTank()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-forward (%d).png", 1, 9, 0.1f), "tank_forward");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-back (%d).png", 1, 9, 0.1f), "tank_back");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-left (%d).png", 1, 9, 0.2f), "tank_left");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-right (%d).png", 1, 9, 0.2f), "tank_right");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-attackLeft (%d).png", 1, 9, 0.2f), "tank_attackLeft");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-attackRight (%d).png", 1, 9, 0.2f), "tank_attackRight");
	AnimationCache::getInstance()->addAnimation(createAnimation("Unit3-die (%d).png", 1, 9, 0.2f), "tank_die");
}

void GameAnimation::initBaseCar()
{
	AnimationCache::getInstance()->addAnimation(createAnimation("BaseCar-forward (%d).png.png", 1, 2, 0.1f), "BaseCar_forward");
	AnimationCache::getInstance()->addAnimation(createAnimation("BaseCar-back (%d).png", 1, 2, 0.1f), "BaseCar_back");
	AnimationCache::getInstance()->addAnimation(createAnimation("BaseCar-left (%d).png", 1, 2, 0.1f), "BaseCar_left");
	AnimationCache::getInstance()->addAnimation(createAnimation("BaseCar-right (%d).png", 1, 2, 0.1f), "BaseCar_right");
}