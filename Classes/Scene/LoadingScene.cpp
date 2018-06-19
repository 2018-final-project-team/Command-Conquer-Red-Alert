/*
*  @file     LoadingScene.cpp
*  @brief    加载场景类，显示的第一个场景，完成资源预加载并显示加载进度
*  @author   王亮
*/

#include "cocos2d.h"
#include "Scene/LoadingScene.h"
#include "Scene/LoginScene.h"
#include "PreloadList.h"
#include "Util/GameAudio.h"
#include "Util/GameAnimation.h"
USING_NS_CC;

Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();

    auto layer = LoadingScene::create();

    scene->addChild(layer);

    return scene;
}

void LoadingScene::endLoading(float dt) {
    const auto transition = TransitionFade::create(1, LoginScene::createScene());
    Director::getInstance()->replaceScene(transition);
}

void LoadingScene::loading()
{
	loadingBar->setPercent(10);

	loadSpriteFrame();
	loadingBar->setPercent(40);

	loadAnimation();
	loadingBar->setPercent(60);

	loadSound();
	loadingBar->setPercent(70);

	loadImage();
	loadingBar->setPercent(100);
}

void LoadingScene::loadAnimation()
{
	GameAnimation::getInstance()->init();
}

void LoadingScene::loadSound()
{
	auto gameAudioInstance = GameAudio::getInstance();

	//=====================初始化背景音乐============================
	for (auto bgm : Preload::bgm)
	{
		gameAudioInstance->preloadBgm(bgm.c_str());
	}

	//=====================初始化音效================================
	for (auto effect : Preload::effect)
	{
		gameAudioInstance->preloadEffect(effect.c_str());
	}
}

void LoadingScene::loadImage() {
    //undefined
}

void LoadingScene::loadSpriteFrame()
{
    auto spriteframecache = SpriteFrameCache::getInstance();

	//c++11 基于范围的for循环
    for (auto plist : Preload::plists)
    {
        spriteframecache->addSpriteFramesWithFile(plist);
    }
}


bool LoadingScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto baseY = visibleSize.height * 0.35f;

	initBackground(origin, visibleSize);
	loadingBar = createLoadingBar();
    addChild(loadingBar);

	loading();
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(LoadingScene::endLoading), 0.5);  //0.5s后执行endLoading()
    return true;
}

void LoadingScene::initBackground(Vec2 origin, Size visibleSize)
{
	auto background = Sprite::create("LoadingScene/loadingbg.png");
	if (background == nullptr)
	{
		log("'loadingbg.png' is not found");
	}
	else
	{

		background->setAnchorPoint(Vec2(0.5, 0.5));
		// position the sprite on the center of the screen
		background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

		// add the sprite as a child to this layer
		this->addChild(background, -1);
	}
}

cocos2d::ui::LoadingBar* LoadingScene::createLoadingBar(){
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto baseY = visibleSize.height * 0.3f;

	//进度条的框
	auto bg = Sprite::create("LoadingScene/loadbarBg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, baseY));
	this->addChild(bg);

    auto loadingBar = ui::LoadingBar::create("LoadingScene/loadbar.png");
    loadingBar->setPosition(Vec2(visibleSize.width / 2, baseY));
    loadingBar->setScale(1);
    loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);

    return loadingBar;
}

