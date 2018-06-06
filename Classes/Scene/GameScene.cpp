#pragma once
#include "Scene/WelcomeScene.h"  
#include "Scene/GameScene.h"  
#include "ui\CocosGUI.h"
#define MINLENTH 15
#define SPEED 20
#define small_mapX 300
#define small_mapY 300
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

	this->dataInit();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//===================Load map=========================
	_tileMap = TMXTiledMap::create("GameItem/Map/mapbeautiful1.tmx");
	this->addChild(_tileMap);

	_barrier = _tileMap->getLayer("barrier");
	/*update by czd*/
	Sprite* small_map = Sprite::create("GameItem/Map/small_map1.png");
	small_map->setContentSize(Size(small_mapX, small_mapY));
	small_map->setPosition(Point(visibleSize.width - small_mapX / 2, visibleSize.height - small_mapY / 2));
	this->addChild(small_map);

	_gameEventDispatcher = Director::getInstance()->getEventDispatcher();

	_gameListener = EventListenerTouchOneByOne::create();
	_gameListener->onTouchBegan = [=](Touch* touch, Event* event) {
		//=========== 点击小地图的移动功能 ===============
		Point position = touch->getLocation();
		if (position.x > visibleSize.width - small_mapX && position.y > visibleSize.height - small_mapY) {
			auto X = (position.x - (visibleSize.width - small_mapX)) / small_mapX * _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.width / 2;
			auto Y = (position.y - (visibleSize.height - small_mapY)) / small_mapY * _tileMap->getMapSize().height*_tileMap->getTileSize().height - visibleSize.width / 2;
			if (X < 0) X = 0;
			if (Y < 0) Y = 0;
			if (X > _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.width) X = _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.width;
			if (Y > _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.height) Y = _tileMap->getMapSize().width*_tileMap->getTileSize().width - visibleSize.height;
			_tileMap->runAction(MoveTo::create(0.1, Point(-X, -Y)));

			return false;
		}
		else {
			log("%f,%f", position.x, position.y);
			if (isCollision(position))
				log("0");
			else
				log("1");
		}

		return true;
	};

	_gameListener->onTouchEnded = [=](Touch* touch, Event* event) {
		_touchEnd = touch->getLocation();
		if (_touchEnd == _touchBegan)      // 点击则判断点击对象
		{
			//生成Sprite的Rect
			auto target = static_cast<Sprite*>(event->getCurrentTarget());
			Vec2 locationInTarget = target->convertToNodeSpace(touch->getLocation());
			Size size = target->getContentSize();
			Rect rect(0, 0, size.width, size.height);

			if (rect.containsPoint(locationInTarget))
			{
				switch (target->getTag())
				{
				case INFANTRY_TAG:
				case DOG_TAG:
				case TANK_TAG:
					_manager->setEnemy(static_cast<Unit*>(target));
					break;
				case POWER_PLANT_TAG:
				case MINE_TAG:
				case CAR_FACTORY_TAG:
				case BASE_TAG:
				case BARRACKS_TAG:
					_manager->setBuilding(static_cast<Building*>(target));
					break;
				default:
					// 为层注册监听器后层也会响应
					log("default");
					// isCollision有bug 故此处会崩
					//_manager->getMoveController()->setDestination(_touchEnd);
				}
			}
		}
		else                              // 矩形框选择士兵
		{
			_manager->getMoveController()->selectSoldiersWithMouse(_touchBegan, _touchEnd);
		}
	};

	_gameEventDispatcher->addEventListenerWithSceneGraphPriority(_gameListener, this);

	/*update by czd */
	auto _mouseOutBoradListener = EventListenerMouse::create();
	_mouseOutBoradListener->onMouseMove = [&](Event* event) {
		EventMouse* pem = static_cast<EventMouse*>(event);
		_cursorPosition = Vec2(pem->getCursorX(), pem->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_mouseOutBoradListener, 1);


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

	scheduleUpdate();


	_manager = Manager::createWithGameScene(this);

	_manager->retain();
	_manager->getMoveController()->retain();

}

void GameScene::onExit()
{
	Layer::onExit();
	_gameEventDispatcher->removeAllEventListeners();
}

void GameScene::dataInit()
{
	// To Do: 数据合理
	_isPowerEnough = false;
	_money = 1000;
	_power = 0;

	_barracksNum = 0;
	_mineNum = 0;
	_powerPlantNum = 0;
	_carFactoryNum = 0;

	_carFactoryPosition = _barracksPosition = Vec2::ZERO;

	_isBaseExist = false;
}

void GameScene::menuBackCallback(Ref *pSender)
{
	//跳转到第一个场景，记得包含第一个场景的头文件：GameScene.h  
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

void GameScene::addPower(int power)
{
	_power += power;
	if (_power >= 0)
	{
		_isPowerEnough = true;
	}
	else
	{
		_isPowerEnough = false;
	}
}

void GameScene::decreasePower(int power)
{
	_power -= power;
	if (_power >= 0)
	{
		_isPowerEnough = true;
	}
	else
	{
		_isPowerEnough = false;
	}
}

void GameScene::addTotalPower(int power)
{
	_totalPower += power;
}

void GameScene::decreaseTotalPower(int power)
{
	_totalPower -= power;
}

void GameScene::update(float time)
{
	_manager->waitCreateBuilding();
	_manager->waitCreateSoldier();

	_manager->attack();
	_manager->addMoneyUpdate();

	_manager->getMoveController()->moveSoldiers();

	scrollMap();

}

/*update by czd */
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
//=========== 传入一个位置，判断是否有障碍，若有，为false ,这个位置是以屏幕左下角为原点===============
bool GameScene::isCollision(cocos2d::Vec2 position1)
{
	// turn PixelPosition to TileCoord
	Size mapSize = _tileMap->getMapSize();
	Size tileSize = _tileMap->getTileSize();
	auto position = _tileMap->convertToNodeSpace(position1);
	if (position.x < 0 || position.y<0 || position.x>_tileMap->getMapSize().width*_tileMap->getTileSize().width || position.y > _tileMap->getMapSize().height*_tileMap->getTileSize().height) {
		return false;
	}
	position.x = static_cast<int>(position.x / tileSize.width);
	position.y = mapSize.height-static_cast<int>(position.y/ tileSize.width)-1;	
	// get the GID of tile
	int tileGID = _barrier->getTileGIDAt(position);
	// get the properties
	if (tileGID != 0) {
		//auto properties = _tileMap->getPropertiesForGID(tileGID).asValueMap();
		//return properties["moveable"].asInt();
		return true;
	}
	return false;

	
}

float GameScene::getTileSize()
{
	return _tileMap->getTileSize().width;
}