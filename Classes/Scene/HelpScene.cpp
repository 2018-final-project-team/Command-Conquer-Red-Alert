/*
*  @file     HelpScene.cpp
*  @brief    甯姪鍦烘櫙绫伙紝鏄剧ず娓告垙鐜╂硶鍜屾搷浣滄柟娉?
*  @author   鐜嬩寒
*/

#include "HelpScene.h"
#include "Settings.h"
#include "Scene/WelcomeScene.h"

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WelcomeScene.cpp\n");
}

Scene* HelpScene::createScene()
{
	auto scene = Scene::create();

	auto layer = HelpScene::create();

	scene->addChild(layer);


	return scene;

}

bool HelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//=====================鏂囨湰label============================
	auto label = Label::createWithTTF("HelpScene!", "fonts/Marker Felt.ttf", 45);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(label, 0);



	//=====================鑳屾櫙鍥剧墖============================
	Sprite * bg = Sprite::create("emptyBg.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg, -1);




	//============to do:娣诲姞娓告垙鐜╂硶鍜屾搷浣滄柟娉曠殑淇℃伅===========





	//=====================杩斿洖鎸夐挳============================
	auto backItem = MenuItemImage::create(
		"backNormal.png",
		"backSelected.png",
		CC_CALLBACK_1(HelpScene::menuBackCallback, this));

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

void HelpScene::menuBackCallback(Ref *pSender)
{
	Director::getInstance()->popScene();
}