#include"Scene/WelcomeScene.h"  
#include"Scene/GameScene.h"  
#include"Data/Building.h"

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WelcomeScene.cpp\n");
}

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
    
    scene -> getPhysicsWorld() -> setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
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


	//----------test Building class----------------------
	auto label = LabelTTF::create("Create Building1", "Courier", 30);
	label->setPosition(Point(150, 150));
	addChild(label);

	auto Listener = EventListenerTouchOneByOne::create();
	Listener->onTouchBegan = [label, this](Touch* t, Event* e) {
		if (label->getBoundingBox().containsPoint(t->getLocation())) {
			auto b1 = Building::create(BASE_TAG);
			b1->setPosition(Vec2(150, 400));
			this->addChild(b1);
			
		}
		return false;
	};

	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(Listener, label);
	//---------------end-----------------------------------




	//auto label = Label::createWithTTF("Hello,This is GameScene", "fonts/Marker Felt.ttf", 45);
	//label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	//this->addChild(label, 0);

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


	

	return true;
}

void GameScene::menuBackCallback(Ref *pSender)
{
	const auto transition = TransitionFade::create(1, WelcomeScene::createScene());
	Director::getInstance()->replaceScene(transition);
}

void GameScene::onEnter()
{
    Layer::onEnter();
    auto listener = EventListenerPhysicsContact::create();
    listener -> onContactBegin = [](PhysicsContact & contact)
    {
        //        auto spriteA = (Sprite *)contact.getShapeA() -> getBody() -> getNode();
        //        auto spriteB = (Sprite *)contact.getShapeB() -> getBody() -> getNode();
        log("onContact");
        return true;
    };
    Director::getInstance() -> getEventDispatcher() -> addEventListenerWithFixedPriority(listener, 1);
}

void GameScene::onExit()
{
    CCLOG("Exit");
    Director::getInstance() -> getEventDispatcher() -> removeAllEventListeners();
}

