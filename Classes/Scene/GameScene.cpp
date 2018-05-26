#include"Scene/WelcomeScene.h"  
#include"Scene/GameScene.h"  

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WelcomeScene.cpp\n");
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);


	return scene;

}

bool GameScene::init()
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

	//·µ»Ø°´Å¥
	auto backItem = MenuItemImage::create(
		"backNormal.png",
		"backSelected.png",
		CC_CALLBACK_1(GameScene::menuBackCallback, this));

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



    manager = Manager::createWithGameScene(this);
	
	return true;
}
void GameScene::menuBackCallback(Ref *pSender)
{
	//Ìø×ªµ½µÚÒ»¸ö³¡¾°£¬¼ÇµÃ°üº¬µÚÒ»¸ö³¡¾°µÄÍ·ÎÄ¼þ£ºGameScene.h  
	//Director::getInstance()->replaceScene(MyFirstScene::createScene());  
	Director::getInstance()->popScene();
}

Vector<Unit*>* GameScene::getSelectedSoldiers()
{
    return &_selectedSoldiers;
}

Vector<Unit*>* GameScene::getSoldiers()
{
    return &_soldiers;
}

Vector<Building*>* GameScene::getBuildings()
{
    return &_buildings;
}

void GameScene::addMoney(int money)
{
    _money += money;
}

void GameScene::decreaseMoney(int money)
{
    _money -= money;
}

void GameScene::addPower()
{
    _power += 100;
}

void GameScene::decreasePower(int power)
{
    _power -= power;
}