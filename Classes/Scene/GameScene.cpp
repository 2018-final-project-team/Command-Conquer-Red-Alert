#include "Scene/WelcomeScene.h"  
#include "Scene/GameScene.h"  
#include "ui\CocosGUI.h"
#include "Panel/Panel.h"
#define small_mapX 300
#define small_mapY 300
#define MINLENTH 15
#define SPEED 20

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
	auto scene = Scene::createWithPhysics();

	//调试用
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

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
    MAPX = _tileMap->getMapSize().width * _tileMap->getTileSize().width;
    MAPY = _tileMap->getMapSize().height * _tileMap->getTileSize().height;
	this->addChild(_tileMap, 0);

	_barrier = _tileMap->getLayer("barrier");

	/*update by czd*/
	Sprite* small_map = Sprite::create("GameItem/Map/small_map1.png"); 
    small_map->setPosition(Point(visibleSize.width - small_mapX / 2, visibleSize.height - small_mapX / 2));
	this->addChild(small_map, 0);


    //DrawNode
    DrawNode* drawNode = DrawNode::create();
    this->addChild(drawNode);

	//=====================添加Panel========================
	auto panel = Panel::createWithGameScene(this);
	if (panel == nullptr) { log("create panel error"); }
	panel->setPosition(visibleSize.width - 112, visibleSize.height - 400);
	this->addChild(panel, 3);

	/*键盘监听 by czd */
	auto _keyboardListener = EventListenerKeyboard::create();
	_keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			_keyUp = true;
			CCLOG("按下了：上方向键");
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			_keyLeft = true;
			//CCLOG("按下了：左方向键");
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			_keyRight = true;
			//CCLOG("按下了：右方向键");
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
			_keyDown = true;
			CCLOG("按下了：下方向键");
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardListener, this);
	auto _keyboardReleasedListener = EventListenerKeyboard::create();
	_keyboardReleasedListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			_keyUp = false;
			CCLOG("松开了：上方向键");
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			_keyLeft = false;
			//CCLOG("按下了：左方向键");
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			_keyRight = false;
			//CCLOG("按下了：右方向键");
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
			_keyDown = false;
			CCLOG("松开了：下方向键");
		}

		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_keyboardReleasedListener, this);


	_gameListener = EventListenerTouchOneByOne::create();
	_gameListener->onTouchBegan = [=](Touch* touch, Event* event) {
		//=========== 点击小地图的移动功能 ===============
		Point position = touch->getLocation();
		if (position.x > visibleSize.width - small_mapX && position.y > visibleSize.height - small_mapY) 
        {
            auto X = (position.x - (visibleSize.width - small_mapX)) / small_mapX * MAPX - visibleSize.width / 2;
            auto Y = (position.y - (visibleSize.height - small_mapY)) / small_mapY * MAPY - visibleSize.width / 2;
			if (X < 0) X = 0;
			if (Y < 0) Y = 0;
            if (X > MAPX - visibleSize.width) 
                X = MAPX - visibleSize.width;
            if (Y > MAPY - visibleSize.height) 
                Y = MAPY - visibleSize.height;
            //direction to move sprites
            Vec2 direction = Point(-X, -Y) - _tileMap->getPosition();
            log("%f %f", -X, -Y);

			_tileMap->setPosition(Point(-X, -Y));
            moveSpritesWithMap(direction);
            
			return false;
		}
		else
		{
			_touchBegan = position;   // 记录起点
		}
		return true;
	};

    // 选择时画出矩形
    _gameListener->onTouchMoved = [=](Touch* touch, Event* event) {
        Point movePosition = touch->getLocation();
        drawNode->clear();
        drawNode->drawRect(_touchBegan, Vec2(_touchBegan.x, movePosition.y), 
            movePosition, Vec2(movePosition.x, _touchBegan.y), Color4F(255, 255, 255, 100));
    };

	_gameListener->onTouchEnded = [=](Touch* touch, Event* event) {
	    _touchEnd = touch->getLocation();
        drawNode->clear();
        if (fabs(_touchEnd.x - _touchBegan.x) < 15.0 && 
            fabs(_touchEnd.y - _touchBegan.y) < 15.0)      // 点击则判断点击对象
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
				case BASE_CAR_TAG:
					if (_selectedSoldiers.contains(static_cast<Unit*>(target)))
					{
						//基地车展开成基地
						//移除基地车
						_selectedSoldiers.clear();
						_soldiers.eraseObject(static_cast<Unit*>(target), false);
						Vec2 position = target->getPosition();
						this->removeChild(target);
						//创建基地
						Building* base = Building::create(BASE_TAG);
						_gameEventDispatcher->addEventListenerWithSceneGraphPriority
						(_gameListener->clone(), base);
						base->setPosition(position);
						this->addChild(base, 2);
						_isBaseExist = true;
						_buildings.pushBack(base);
					}

                default:
                    // 为层注册监听器后层也会响应 所以此处需要判断士兵建筑和空地
                    log("default");
                    // 测试 isCollision
                    //log("%d", isCollision(_touchEnd));
                    _manager->getMoveController()->setDestination(_touchEnd);


					////====测试血条功能=====
					//for (auto& soldier : _selectedSoldiers)
					//{
					//	soldier->decreaseHP(20);
					//}

                }
            }
        }
		else                              // 矩形框选择士兵
		{
			_manager->getMoveController()->selectSoldiersWithMouse(_touchBegan, _touchEnd);
		}
    };

	_gameEventDispatcher = Director::getInstance()->getEventDispatcher();
	_gameEventDispatcher->addEventListenerWithSceneGraphPriority(_gameListener, this);

	/*update by czd */
	auto _mouseOutBoradListener = EventListenerMouse::create();
	_mouseOutBoradListener->onMouseMove = [&](Event* event) {
		EventMouse* pem = static_cast<EventMouse*>(event);
		_cursorPosition = Vec2(pem->getCursorX(), pem->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_mouseOutBoradListener, 1);



	//===================添加基地车==========================
	auto baseCar = Unit::create(BASE_CAR_TAG);
	baseCar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(baseCar, 1);
	baseCar->setGetDestination(true);
	_gameEventDispatcher->addEventListenerWithSceneGraphPriority
	(_gameListener->clone(), baseCar);
	_soldiers.pushBack(baseCar);
	log("%f %f", baseCar->getPosition().x, baseCar->getPosition().y);



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


	scheduleUpdate();


	_manager = Manager::createWithGameScene(this);

	//由于GameScene中对panel指针的内存管理存在暂时无法解决的问题，所以采用直接向manager传递指针的方式
	_manager->setPanel(panel);

	_manager->retain();
	_manager->getMoveController()->retain();

	panel->retain();
	panel->_powerPlantIcon->retain();
	panel->_mineIcon->retain();
	panel->_barracksIcon->retain();
	panel->_carFactoryIcon->retain();
	panel->_infantryIcon->retain();
	panel->_dogIcon->retain();
	panel->_tankIcon->retain();

	return true;
}

void GameScene::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = [](PhysicsContact & contact)
	{
		//        auto spriteA = (Sprite *)contact.getShapeA() -> getBody() -> getNode();
		//        auto spriteB = (Sprite *)contact.getShapeB() -> getBody() -> getNode();
		log("onContact");
		return true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
}

void GameScene::onExit()
{
	Layer::onExit();
	_gameEventDispatcher->removeEventListener(_gameListener);
}

void GameScene::dataInit()
{
	// To Do: 数据合理
	_isPowerEnough = false;
	_money = 2000;
	_power = 0;

	_barracksNum = 0;
	_mineNum = 0;
	_powerPlantNum = 0;
	_carFactoryNum = 0;

    _tankNum = 0;
    _infantryNum = 0;
    _dogNum = 0;

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
    Layer::update(time);

	_manager->attack();
	_manager->addMoneyUpdate();

    _manager->waitCreateBuilding();
    _manager->waitCreateSoldier();
    _manager->waitCreateCar();

	_manager->getMoveController()->moveSoldiers();

	scrollMap();

}

/*update by czd */
void GameScene::scrollMap() 
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto X = _cursorPosition.x;
	auto Y = _cursorPosition.y;
    Point mapPosition = _tileMap->getPosition();
	if (X < MINLENTH || _keyLeft)
    {
		if (_tileMap->getPositionX() + SPEED < 0) 
        {
			_tileMap->setPosition(Vec2(SPEED, 0) + mapPosition);
            moveSpritesWithMap(Vec2(SPEED, 0));
		}
		else 
        {
			_tileMap->setPositionX(0);
            moveSpritesWithMap(Vec2(-mapPosition.x, 0));
		}
	}
    else if (X > visibleSize.width - MINLENTH || _keyRight)
    {
        if (_tileMap->getPositionX() - SPEED > -MAPX + visibleSize.width)
        {
            _tileMap->setPosition(mapPosition + Vec2(-SPEED, 0));
            moveSpritesWithMap(Vec2(-SPEED, 0));
        }
        else 
        {
            _tileMap->setPositionX(-MAPX + visibleSize.width);
            moveSpritesWithMap(Vec2(-MAPX + visibleSize.width - mapPosition.x, 0));
        }
    }

	if (Y < MINLENTH || _keyDown)
    {
		if (_tileMap->getPositionY() + SPEED < 0) 
        {
			_tileMap->setPosition(mapPosition + Point(0, SPEED));
            moveSpritesWithMap(Vec2(0, SPEED));
		}
		else 
        {
			_tileMap->setPositionY(0);
            moveSpritesWithMap(Vec2(0, -mapPosition.y));
		}
	}
	else if (Y > visibleSize.height - MINLENTH || _keyUp)
    {
		if (_tileMap->getPositionY() - SPEED >  -MAPY + visibleSize.height)
        {
			_tileMap->setPosition(Vec2(0, -SPEED) + mapPosition);
            moveSpritesWithMap(Vec2(0, -SPEED));
		}
		else 
        {
			_tileMap->setPositionY(-MAPY + visibleSize.height);
            moveSpritesWithMap(Vec2(0, -MAPY + visibleSize.height - mapPosition.y));
		}
	}
}

void GameScene::moveSpritesWithMap(cocos2d::Vec2 direction)
{
    // my soldiers
    for (auto& soldier : _soldiers)
    {
        soldier->setPosition(soldier->getPosition() + direction);
        std::vector<Point>::iterator iter2;
        for (iter2 = soldier->_route.begin(); iter2 != soldier->_route.end(); iter2++)
        {
            *iter2 += direction;
        }
    }
    // my buildings
    for (auto& building : _buildings)
    {
        building->setPosition(building->getPosition() + direction);
    }
    // enemy soldiers
    for (auto& soldier : _enemySoldiers)
    {
        soldier->setPosition(soldier->getPosition() + direction);
    }
    // enemy buildings
    for (auto& building : _enemyBuildings)
    {
        building->setPosition(building->getPosition() + direction);
    }
    // barracks position
    if (_barracksNum)
    {
        _barracksPosition += direction;
    }
    // car factory position
    if (_carFactoryNum)
    {
        _carFactoryPosition += direction;
    }
}

bool GameScene::isCollision(cocos2d::Vec2 position1)
{
    // turn PixelPosition to TileCoord
    Size mapSize = _tileMap->getMapSize();
    Size tileSize = _tileMap->getTileSize();
    auto position = _tileMap->convertToNodeSpace(position1);
    if (position.x < 0 || position.y<0 || position.x > MAPX
        || position.y > MAPY) 
    {
        return false;
    }
    position.x = static_cast<int>(position.x / tileSize.width);
    position.y = mapSize.height - static_cast<int>(position.y / tileSize.width) - 1;
    // get the GID of tile
    int tileGID = _barrier->getTileGIDAt(position);

    if (!tileGID) 
    {
        return true;
    }
    return false;
}

float GameScene::getTileSize()
{
	return _tileMap->getTileSize().width;
}
