﻿#include "Scene/LoadingScene.h"
#include "Scene/LoginScene.h"
//#include "Settings.h"
#include "SimpleAudioEngine.h"
#include "PreloadList.h"

USING_NS_CC;

Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void LoadingScene::endLoading(float dt) {
    const auto transition = TransitionFade::create(1, LoginScene::createScene());
    Director::getInstance()->replaceScene(transition);
}

void LoadingScene::loadSound()
{
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

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto baseY = visibleSize.height * 0.35f;

    addChild(createLoadingBar());
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(LoadingScene::endLoading), 0.5);  //0.5s后执行endLoading()
    return true;
}

cocos2d::ui::LoadingBar* LoadingScene::createLoadingBar(){
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto baseY = visibleSize.height * 0.35f;

    auto loadingBar = ui::LoadingBar::create("StartScene/loading bar.jpg");
    loadingBar->setPosition(Vec2(visibleSize.width / 2, baseY));
    loadingBar->setScale(0.5);
    loadingBar->setDirection(ui::LoadingBar::Direction::LEFT);
    loadingBar->setPercent(10);

    loadSpriteFrame();
    loadingBar->setPercent(40);
    
    loadImage();
    loadingBar->setPercent(70);

    loadSound();
    loadingBar->setPercent(100);
   
    return loadingBar;
}
