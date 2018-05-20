#include "Scene/WelcomeScene.h"
#include "Scene/GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();

	auto layer = WelcomeScene::create();

	scene->addChild(layer);

	return scene;

}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WelcomeScene.cpp\n");
}

// on "init" you need to initialize your instance
bool WelcomeScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"myClose1.png",
		"myClose2.png",
		CC_CALLBACK_1(WelcomeScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'closeNormal.png' and 'closeSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	//add a menu item with "X" image, which is clicked to enter the GameScene
	//add a "start" icon to enter the GameScene. it's an autorelease object
	auto startItem = MenuItemImage::create(
		"StartNormal.png",
		"StartSelected.png",
		CC_CALLBACK_1(WelcomeScene::enterGameScene, this));

	if (startItem == nullptr ||
		startItem->getContentSize().width <= 0 ||
		startItem->getContentSize().height <= 0)
	{
		problemLoading("'StartNormal.png' and 'StartSelected.png'");
	}
	else
	{
		startItem->setScale(0.75);
		float x = origin.x + visibleSize.width / 2;
		float y = origin.y + visibleSize.height / 2 - 70;
		startItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu2 = Menu::create(startItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Red Alert Demo"
	// create and initialize a label

	auto label = Label::createWithTTF("Red Alert Demo", "fonts/Marker Felt.ttf", 40);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		// add the label as a child to this layer
		this->addChild(label, 1);
	}



	auto background = Sprite::create("background.png");
	if (background == nullptr)
	{
		problemLoading("'background.png'");
	}
	else
	{

		background->setAnchorPoint(Vec2(0.5, 0.5));
		// position the sprite on the center of the screen
		background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

		//background->setScale(1.5);
		// add the sprite as a child to this layer
		this->addChild(background, -2);
	}

	auto logo = Sprite::create("logo1.png");
	if (logo == nullptr)
	{
		problemLoading("'logo.png'");
	}
	else
	{
		logo->setScale(0.75);
		// position the sprite on the center of the screen
		logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 105));
		// add the sprite as a child to this layer
		this->addChild(logo, 2);
	}



	//====================场景中的动画效果============================
	//1.爆炸动画
	Vector<SpriteFrame*> animFramesExplosion = getAnimation("Animation/explosion.plist", "explosion_air_%d.png", 10);
	auto explosion = Sprite::createWithSpriteFrame(animFramesExplosion.front());
	explosion->setPosition(350, 100);

	// create the animation out of the frames
	Animation* animationExplosion = Animation::createWithSpriteFrames(animFramesExplosion, 0.125f);
	Animate* animateExplosion = Animate::create(animationExplosion);

	// run it and repeat it forever
	explosion->runAction(RepeatForever::create(animateExplosion));

	this->addChild(explosion, 3);

	//2.龙卷风动画
	Vector<SpriteFrame*> animFramesWind = getAnimation("Animation/wind.plist", "ArchMageTwister_%d.png", 12);
	auto wind = Sprite::createWithSpriteFrame(animFramesWind.front());
	wind->setPosition(650, 175);

	// create the animation out of the frames
	Animation* animationWind = Animation::createWithSpriteFrames(animFramesWind, 0.125f);
	Animate* animateWind = Animate::create(animationWind);

	// run it and repeat it forever
	wind->runAction(RepeatForever::create(animateWind));

	this->addChild(wind, 3);



	return true;
}


void WelcomeScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}


void WelcomeScene::enterGameScene(Ref *pSender)
{
	//使用Director类实现场景的跳转 
	// Director::getInstance()->end();  
	Director::getInstance()->replaceScene(GameScene::createScene());  
}

Vector<SpriteFrame*> WelcomeScene::getAnimation(const char* sheetPlistName, const char* format, int count)
{
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile(sheetPlistName);
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	return animFrames;
}