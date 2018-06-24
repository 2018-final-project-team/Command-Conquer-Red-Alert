/*
*  @file     EndingScene.cpp
*  @brief    结束场景
*  @author   王星洲
*/
#include "Scene/EndingScene.h"
#include "Scene/WelcomeScene.h"


USING_NS_CC;


Scene* EndingScene::createScene()
{
	auto scene = Scene::create();

	auto layer = EndingScene::create();

	scene->addChild(layer);

	return scene;
}


// on "init" you need to initialize your instance
bool EndingScene::init()
{
	
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//Back to home
	auto homeItem = MenuItemImage::create(
		"EndingScene/HomeNormal.png","EndingScene/HomeSelected.png",
		CC_CALLBACK_1(EndingScene::menuWelcomeCallback, this));

	if (homeItem == nullptr ||
		homeItem->getContentSize().width <= 0 ||
		homeItem->getContentSize().height <= 0)
	{
		CCLOG("Error to loading home Item!");
	}
	else
	{
		float x = origin.x + visibleSize.width - homeItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height*2/3 - homeItem->getContentSize().height / 2;
		homeItem->setPosition(Vec2(x, y));
	}

	auto menu = Menu::create(homeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	//close

	auto closeItem = MenuItemImage::create(
		"myClose1.png",
		"myClose2.png",
		CC_CALLBACK_1(EndingScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		CCLOG("Error to loading close Item!");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	auto closeMenu = Menu::create(closeItem, NULL);
	closeMenu->setPosition(Vec2::ZERO);
	this->addChild(closeMenu, 1);

	//=====================label："Win or Lose"============================
	// Todo:judge for win/lose
	auto label = Label::createWithTTF("Judge is not done", "fonts/Marker Felt.ttf", 40);
	
	if (isWinner())
	{
		label = Label::createWithTTF("Congradulations", "fonts/Marker Felt.ttf", 40);
	}
	else if (isLoser())
	{
		label = Label::createWithTTF("Defeated", "fonts/Marker Felt.ttf", 40);
	}

	if (label == nullptr)
	{
		CCLOG("label loading error");
	}
	else
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		this->addChild(label, 1);
	}

	//=====================label："Game Time"============================
	auto timeLabel = Label::createWithTTF("GameTime:", "fonts/Marker Felt.ttf", 40);
	timeLabel->setTextColor(Color4B::BLACK);
	if (timeLabel == nullptr)
	{
		CCLOG("label loading error");
	}
	else
	{
		timeLabel->setPosition(Vec2(origin.x + visibleSize.width / 3,
			origin.y + visibleSize.height/2 - timeLabel->getContentSize().height));

		this->addChild(timeLabel, 1);
	}
	//=====================label："Score"============================
	// Todo:judge for win/lose
	auto scoreLabel = Label::createWithTTF("GameScore:", "fonts/Marker Felt.ttf", 40);
	scoreLabel->setTextColor(Color4B::BLACK);
	if (scoreLabel == nullptr)
	{
		CCLOG("label loading error");
	}
	else
	{
		scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 3,
			origin.y + visibleSize.height/3 - scoreLabel->getContentSize().height));

		this->addChild(scoreLabel, 1);
	}

	//=====================背景图片============================
	auto background = Sprite::create("EndingScene/endingSceneBg.png");
	if (background == nullptr)
	{
		CCLOG("background picture loading error");
	}
	else
	{
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

		this->addChild(background, 0);
	}

	return true;
}


void EndingScene::menuCloseCallback(Ref* pSender)
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


void EndingScene::menuWelcomeCallback(Ref *pSender)
{
	Director::getInstance()->pushScene(TransitionFade::create(1, WelcomeScene::createScene()));
}


bool EndingScene::isWinner()
{
	return false;
}

bool EndingScene::isLoser()
{
	return true;
}