/*
*  @file     WelcomeScene.cpp
*  @brief    欢迎（主）场景类，可通过此场景进入游戏、设置、帮助场景，可退出游戏
*  @author   王亮
*/

#include "Scene/WelcomeScene.h"
#include "Scene/GameScene.h"
#include "Scene/SettingsScene.h"
#include "Scene/HelpScene.h"
#include "Util/GameAnimation.h"
#include "Scene\NetMenu.h"

USING_NS_CC;

static std::string _userName;


Scene* WelcomeScene::createScene()
{
	auto scene = Scene::create();

	auto layer = WelcomeScene::create();

	scene->addChild(layer);

	return scene;

}

Scene* WelcomeScene::createScene(std::string playerName)
{
	auto scene = Scene::create();

	_userName = playerName;

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

	//=====================该场景中的4个menu============================
	//=====================“关闭”menu============================
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

	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	//=====================“开始”menu============================
	auto startItem = MenuItemImage::create(
		"StartNormal.png",
		"StartSelected.png",
		CC_CALLBACK_1(WelcomeScene::menuNetCallback, this));

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

	auto menu2 = Menu::create(startItem, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);


	//=====================“设置”menu============================
	auto settingItem = MenuItemImage::create(
		"settingButton.png",
		"settingButton.png",
		CC_CALLBACK_1(WelcomeScene::menuSettingsCallback, this));

	if (settingItem == nullptr ||
		settingItem->getContentSize().width <= 0 ||
		settingItem->getContentSize().height <= 0)
	{
		problemLoading("'settingButton.png' and 'settingButton.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - settingItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - settingItem->getContentSize().height / 2-180;
		settingItem->setPosition(Vec2(x, y));
	}

	auto settingMenu = Menu::create(settingItem, NULL);
	settingMenu->setPosition(Vec2::ZERO);
	this->addChild(settingMenu, 1);


	//=====================“帮助”menu============================
	auto helpItem = MenuItemImage::create(
		"helpButton.png",
		"helpButton.png",
		CC_CALLBACK_1(WelcomeScene::menuHelpCallback, this));

	if (helpItem == nullptr ||
		helpItem->getContentSize().width <= 0 ||
		helpItem->getContentSize().height <= 0)
	{
		problemLoading("'helpButton.png' and 'helpButton.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - helpItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - helpItem->getContentSize().height / 2 -300;
		helpItem->setPosition(Vec2(x, y));
	}

	auto helpMenu = Menu::create(helpItem, NULL);
	helpMenu->setPosition(Vec2::ZERO);
	this->addChild(helpMenu, 1);
	//=====================================================



	//=====================label："Red Alert Demo"============================
	auto label = Label::createWithTTF("Red Alert Demo", "fonts/Marker Felt.ttf", 40);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label->getContentSize().height));

		this->addChild(label, 1);
	}


	//=====================背景图片============================
	auto background = Sprite::create("background.png");
	if (background == nullptr)
	{
		problemLoading("'background.png'");
	}
	else
	{
		background->setAnchorPoint(Vec2(0.5, 0.5));
		background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

		this->addChild(background, -2);
	}

	//=====================logo图片============================
	auto logo = Sprite::create("logo1.png");
	if (logo == nullptr)
	{
		problemLoading("'logo.png'");
	}
	else
	{
		logo->setScale(0.75);
		logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 105));
		// add the sprite as a child to this layer
		this->addChild(logo, 2);
	}


	//====================场景中的动画效果============================
	//1.爆炸动画
	Vector<SpriteFrame*> animFramesExplosion = GameAnimation::getAnimation("explosion_air_%d.png", 10);
	auto explosion = Sprite::createWithSpriteFrame(animFramesExplosion.front());
	explosion->setPosition(350, 100);

	// create the animation out of the frames
	Animation* animationExplosion = Animation::createWithSpriteFrames(animFramesExplosion, 0.125f);
	Animate* animateExplosion = Animate::create(animationExplosion);

	// run it and repeat it forever
	explosion->runAction(RepeatForever::create(animateExplosion));

	this->addChild(explosion, 3);

	//2.龙卷风动画
	Vector<SpriteFrame*> animFramesWind = GameAnimation::getAnimation("ArchMageTwister_%d.png", 12);
	auto wind = Sprite::createWithSpriteFrame(animFramesWind.front());
	wind->setPosition(650, 175);

	// create the animation out of the frames
	Animation* animationWind = Animation::createWithSpriteFrames(animFramesWind, 0.125f);
	Animate* animateWind = Animate::create(animationWind);

	// run it and repeat it forever
	wind->runAction(RepeatForever::create(animateWind));

	this->addChild(wind, 3);
	//=======================================================


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


void WelcomeScene::menuNetCallback(Ref *pSender)
{
	Director::getInstance()->pushScene(TransitionFade::create(1, NetMenu::createScene(_userName)));
}

void WelcomeScene::menuSettingsCallback(cocos2d::Ref * pSender) {
	Director::getInstance()->pushScene(TransitionFade::create(1, SettingsScene::createScene()));
}

void WelcomeScene::menuHelpCallback(cocos2d::Ref * pSender) {
	Director::getInstance()->pushScene(TransitionFade::create(1, HelpScene::createScene()));
}