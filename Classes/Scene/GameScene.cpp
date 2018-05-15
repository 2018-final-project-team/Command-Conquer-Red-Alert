#include"Scene/WelcomeScene.h"  
#include"Scene/GameScene.h"  

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WelcomeScene.cpp\n");
}

Scene* Game::createScene()
{
	auto scene = Scene::create();

	auto layer = Game::create();

	scene->addChild(layer);


	return scene;

}

bool Game::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF("Hello,This is GameScene", "fonts/Marker Felt.ttf", 45);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(label, 0);

	//返回按钮
	auto backItem = MenuItemImage::create(
		"backNormal.png",
		"backSelected.png",
		CC_CALLBACK_1(Game::enterWelcomeScene, this));

	if (backItem == nullptr ||
		backItem->getContentSize().width <= 0 ||
		backItem->getContentSize().height <= 0)
	{
		problemLoading("'backNormal.png' and 'backSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - backItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - backItem->getContentSize().height / 2;
		backItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	

	return true;
}
void Game::enterWelcomeScene(Ref *pSender)
{
	//跳转到第一个场景，记得包含第一个场景的头文件：GameScene.h  
	//Director::getInstance()->replaceScene(MyFirstScene::createScene());  
	Director::getInstance()->popScene();
}