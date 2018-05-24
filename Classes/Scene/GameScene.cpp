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


	//===================Load map=========================
	_tileMap = TMXTiledMap::create("GameItem/map/map1.tmx");
	this->addChild(_tileMap);








	auto label = Label::createWithTTF("Hello,This is GameScene", "fonts/Marker Felt.ttf", 45);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(label, 0);





	//==============为了在此场景中调试地图，暂时注释掉返回按钮，merge前取消注释=======
	////返回按钮
	//auto backItem = MenuItemImage::create(
	//	"backNormal.png",
	//	"backSelected.png",
	//	CC_CALLBACK_1(GameScene::menuBackCallback, this));

	//if (backItem == nullptr ||
	//	backItem->getContentSize().width <= 0 ||
	//	backItem->getContentSize().height <= 0)
	//{
	//	problemLoading("'backNormal.png' and 'backSelected.png'");
	//}
	//else
	//{
	//	float x = origin.x + visibleSize.width - backItem->getContentSize().width / 2;
	//	float y = origin.y + visibleSize.height - backItem->getContentSize().height / 2;
	//	backItem->setPosition(Vec2(x, y));
	//}

	//// create menu, it's an autorelease object
	//auto menu = Menu::create(backItem, NULL);
	//menu->setPosition(Vec2::ZERO);
	//this->addChild(menu, 1);


	//更新函数  
	scheduleUpdate();

	return true;
}

void GameScene::menuBackCallback(Ref *pSender)
{
	const auto transition = TransitionFade::create(1, WelcomeScene::createScene());
	Director::getInstance()->replaceScene(transition);
}



void GameScene::update(float time)
{
	//每一帧地图向左移动5个像素  
	_tileMap->setPositionY(_tileMap->getPositionY() - 5);

	//当第一张地图的最右端和窗口的最左端重合时  
	if (_tileMap->getPositionY() + _tileMap->getContentSize().height <= 0)
	{
		//将第一张地图位置重置
		_tileMap->setPositionY(_tileMap->getPositionY() + _tileMap->getContentSize().height);
	}

}