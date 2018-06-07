/*
*  @file     GameScene.cpp
*  @brief    游戏场景类
*  @author   王亮 蔡震栋
*/

#include "Scene/WelcomeScene.h"  
#include "Scene/GameScene.h"  
#include "ui\CocosGUI.h"
#define MINLENTH 15
#define SPEED 20
#define small_mapX 300
#define small_mapY 300
USING_NS_CC;
using namespace ui;

USING_NS_CC;
using namespace ui;

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
	_tileMap = TMXTiledMap::create("GameItem/Map/mapbeautiful1.tmx");
	this->addChild(_tileMap);

	_barrier = _tileMap->getLayer("barrier");
	/*update by czd*/
	Sprite* small_map = Sprite::create("GameItem/map/small_map1.png");
	small_map->setContentSize(Size(small_mapX, small_mapY));
	small_map->setPosition(Point(visibleSize.width - small_mapX / 2, visibleSize.height - small_mapX / 2));
	this->addChild(small_map);

	auto _smallMapListener = EventListenerTouchOneByOne::create();
	_smallMapListener->onTouchBegan = [=](Touch* touch, Event* event) {
		//=========== 点击小地图的移动功能 ===============
		Point position = touch->getLocation();
		if (position.x > visibleSize.width - small_mapX && position.y > visibleSize.height - small_mapY) {
			auto X = (position.x - (visibleSize.width - small_mapX)) / small_mapX * _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.width / 2;
			auto Y = (position.y - (visibleSize.height - small_mapY))  / small_mapY * _tileMap->getMapSize().height*_tileMap->getTileSize().height - visibleSize.width / 2;
			if (X < 0) X = 0;
			if (Y < 0) Y = 0;
			if (X > _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.width) X = _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.width;
			if (Y > _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.height) Y = _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.height;
			_tileMap->runAction(MoveTo::create(0.1, Point(-X, -Y)));

			return false;
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_smallMapListener, this);



	/*鼠标移到边上地图移动 by czd */
	auto _mouseOutBoradListener = EventListenerMouse::create();
	_mouseOutBoradListener->onMouseMove = [&](Event* event) {
		EventMouse* pem = static_cast<EventMouse*>(event);
		_cursorPosition = Vec2(pem->getCursorX(), pem->getCursorY());
		//log("HelloWorldScene onTouchBegan! pos3 x=%f, y=%f", _cursorPosition.x, _cursorPosition.y);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_mouseOutBoradListener, 1);




	//=====================返回按钮============================
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
		float x = origin.x + backItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - backItem->getContentSize().height / 2;
		backItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);


	//更新函数  
	scheduleUpdate();

	return true;
}

void GameScene::update(float time)
{
	scrollMap();
}

/*鼠标移到边上地图移动 by czd */
void GameScene::scrollMap() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto X = _cursorPosition.x;
	auto Y = _cursorPosition.y;
	if (X < MINLENTH) {
		if (_tileMap->getPositionX() + SPEED < 0) {
			_tileMap->runAction(MoveBy::create(0.1, Point(SPEED, 0)));
		}
		else {
			_tileMap->setPositionX(0);
		}
	}
	if (Y < MINLENTH) {
		if (_tileMap->getPositionY() + SPEED < 0) {
			_tileMap->runAction(MoveBy::create(0.1, Point(0, SPEED)));
		}
		else {
			_tileMap->setPositionY(0);
		}
	}
	if (X > visibleSize.width - MINLENTH) {
		if (_tileMap->getPositionX() - SPEED > -_tileMap->getMapSize().width*_tileMap->getTileSize().width + visibleSize.width) {
			_tileMap->runAction(MoveBy::create(0.1, Point(-SPEED, 0)));
		}
		else {
			_tileMap->setPositionX(-_tileMap->getMapSize().width*_tileMap->getTileSize().width + visibleSize.width);
		}
	}
	if (Y >visibleSize.height - MINLENTH) {
		if (_tileMap->getPositionY() - SPEED > -_tileMap->getMapSize().height*_tileMap->getTileSize().height + visibleSize.height) {
			_tileMap->runAction(MoveBy::create(0.1, Point(0, -SPEED)));
		}
		else {
			_tileMap->setPositionY(-_tileMap->getMapSize().height*_tileMap->getTileSize().height + visibleSize.height);
		}
	}
}



void GameScene::menuBackCallback(Ref *pSender)
{
	const auto transition = TransitionFade::create(1, WelcomeScene::createScene());
	Director::getInstance()->replaceScene(transition);
}