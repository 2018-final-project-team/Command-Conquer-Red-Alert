/*
*  @file     NetMenu.cpp
*  @brief    开始游戏到进入房间方式的选择
*  @brief    功能：选择两种连接方式，选择创建或加入房间
*  @author   wxz
*/

#include <stdio.h>
#include "Scene/NetMenu.h"
#include "Scene/SearchScene.h"
#include "ui/CocosGUI.h"
#include "Scene/WelcomeScene.h"
#include "Settings.h"
#include "Util/GameAudio.h"


USING_NS_CC;

using namespace ui;

static std::string userName;

Scene* NetMenu::createScene(std::string playerName)
{
	userName = playerName;
    auto scene = Scene::create();
	auto layer = NetMenu::create();
    scene->addChild(layer);
    
    return scene;
}

bool NetMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

	_playerName = userName;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("background.png");
    this->addChild(bg);
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));

    selectMapIndex = 0;


	auto remote_button = Button::create("button_normal.png", "button_select.png");
    auto local_button = Button::create("button_normal.png", "button_select.png");
	auto return_button = Button::create("backNormal.png", "backSelected.png");
	remote_button->setTitleText("manual input IP");
	local_button->setTitleText("auto search IP");
	remote_button->setTitleFontSize(20);
	local_button->setTitleFontSize(20);
	remote_button->setScale(1.5);
	local_button->setScale(1.5);
	return_button->setScale(1.5);

	//=============================remote_button===========================
    remote_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 5 * 3));

    remote_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
			GameAudio::getInstance()->playEffect("Sound/button.mp3");
			initIPBox();
			remote_button->setVisible(false);
			local_button->setVisible(false);
			return_button->setVisible(false);
			auto IPButton = Button::create("button_normal.png", "button_selected.png");
			auto returnButton = Button::create("backNormal.png", "backSelected.png");
			IPButton->setTitleText("landlord's IP");
			IPButton->setTitleFontName(Settings::Font::Type::official);
			IPButton->setTitleFontSize(Settings::Font::Size::label);
			IPButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
			returnButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.15f));
			IPButton->setOpacity(233);

			returnButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
			{
				if (type == Widget::TouchEventType::ENDED)
				{
					GameAudio::getInstance()->playEffect("Sound/button.mp3");
					Director::getInstance()->pushScene(TransitionFade::create(1, NetMenu::createScene(_playerName)));
				}
			});

			IPButton->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
			{
				if (type != Widget::TouchEventType::ENDED)
                {
                    return;
                }
                else
                {
                    GameAudio::getInstance()->playEffect("Sound/button.mp3");
                }
				IP = IPInput->getString();
				//log("IP    IP   %c", IP[0]);
				//log("IP    IP   %c", IP[4]);
				if (IP.empty())
				{
					MessageBox("IP can't be empty", "Alert");
					return;
				}
				else
				{
					IPBG->setVisible(false);
					IPInput->setVisible(false);
					returnButton->setVisible(false);
					IP.substr(0, 16);
					UserDefault::getInstance()->setStringForKey("IP", IP);

					//透明黑底板
					Color4B black = Color4B(0, 0, 0, 100);
					auto blackLayer = LayerColor::create(black, 1024, 860);
					blackLayer->setPosition(Vec2(origin.x, origin.y));

					this->addChild(blackLayer, 2);

					auto selector = Sprite::create("BlankBoard.png");
					selector->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
					blackLayer->addChild(selector);

					auto back_button = Button::create("backNormal.png", "backSelected.png");
					auto create_button = Button::create("button_normal.png", "button_selected.png");
					auto join_button = Button::create("button_normal.png", "button_selected.png");

					selector->addChild(create_button);
					selector->addChild(join_button);
					blackLayer->addChild(back_button);

					create_button->setTitleText("create room");
					create_button->setTitleFontSize(20);
					join_button->setTitleText("join room");
					join_button->setTitleFontSize(20);

					create_button->setPosition(Vec2(selector->getContentSize().width / 2, selector->getContentSize().height / 3 * 2));

					join_button->setPosition(Vec2(selector->getContentSize().width / 2, selector->getContentSize().height / 3));

					back_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.1));

					back_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
						if (type == Widget::TouchEventType::ENDED) {
							GameAudio::getInstance()->playEffect("Sound/button.mp3");
							removeChild(blackLayer);
							IPBG->setVisible(true);
							IPInput->setVisible(true);
							returnButton->setVisible(true);
						}
					});

					join_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
						if (type == Widget::TouchEventType::ENDED) {
							GameAudio::getInstance()->playEffect("Sound/button.mp3");
							auto client = Client::create(1);
							client->_landlordIP = IP;
							client->_with_server = false;
							auto transition = TransitionSlideInL::create(0.5, SearchScene::createScene(client, _playerName));

							Director::getInstance()->pushScene(transition);
						}
					});

					create_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
						if (type == Widget::TouchEventType::ENDED) {
							GameAudio::getInstance()->playEffect("Sound/button.mp3");
							auto server = LocalServer::create();
							this->addChild(server);
							auto client = Client::create(1);
							this->addChild(client);
							client->_filter_mode = true;
							client->sensitive_word = _playerName;
							client->_with_server = true;

							auto transition = TransitionSlideInL::create(0.5, RoomScene::createScene(client, 2, _playerName));
							Director::getInstance()->pushScene(transition);

						}
					});


				}
			});
			addChild(IPButton);
			addChild(returnButton);
        }
    });
    this->addChild(remote_button, 1);
    
//=============================local_button===========================
    
    local_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 5 * 2));
    local_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
			GameAudio::getInstance()->playEffect("Sound/button.mp3");
			remote_button->setVisible(false);
			local_button->setVisible(false);
			return_button->setVisible(false);
            Color4B black = Color4B(0, 0, 0, 100);
            auto blackLayer = LayerColor::create(black, 1024, 860);
            blackLayer->setPosition(Vec2(origin.x, origin.y));
            
            this->addChild(blackLayer, 1);
            
            
            auto selector = Sprite::create("BlankBoard.png");
            selector->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            blackLayer->addChild(selector, 1);
            
			auto back_button = Button::create("backNormal.png", "backSelected.png");
			auto create_button = Button::create("button_normal.png", "button_selected.png");
			auto join_button = Button::create("button_normal.png", "button_selected.png");

			selector->addChild(create_button);
			selector->addChild(join_button);
			blackLayer->addChild(back_button);

			create_button->setTitleText("create room");
			create_button->setTitleFontSize(20);
			join_button->setTitleText("join room");
			join_button->setTitleFontSize(20);

			create_button->setPosition(Vec2(selector->getContentSize().width / 2, selector->getContentSize().height / 3 * 2));

			join_button->setPosition(Vec2(selector->getContentSize().width / 2, selector->getContentSize().height / 3));

			back_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.1));
            
            back_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
                if (type == Widget::TouchEventType::ENDED) {
					GameAudio::getInstance()->playEffect("Sound/button.mp3");
					this->removeChild(blackLayer);
					remote_button->setVisible(true);
					local_button->setVisible(true);
					return_button->setVisible(true);
                    
                }
            });
            
            join_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
                if (type == Widget::TouchEventType::ENDED) {
					GameAudio::getInstance()->playEffect("Sound/button.mp3");
                    auto client = Client::create(2);
                    client->_with_server = false;
                    auto transition = TransitionSlideInL::create(0.5, SearchScene::createScene(client, _playerName));
                    
                    Director::getInstance()->pushScene(transition);
                }
            });
            
            create_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
                if (type == Widget::TouchEventType::ENDED) {
					GameAudio::getInstance()->playEffect("Sound/button.mp3");
                    auto server = LocalServer::create();
                    this->addChild(server, 1);
                    auto client = Client::create(2);
                    this->addChild(client, 1);
                    client->_filter_mode = true;
                    client->sensitive_word = _playerName;
                    client->_with_server = true;
                    
                    auto transition = TransitionSlideInL::create(0.5, RoomScene::createScene(client, 2, _playerName));
                    Director::getInstance()->pushScene(transition);
                    
                }
            });

        }
    });

    this->addChild(local_button, 1);

//=============================return button===========================
    
    return_button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height*0.2));
    
    return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
			GameAudio::getInstance()->playEffect("Sound/button.mp3");
			Director::getInstance()->pushScene(TransitionFade::create(1, WelcomeScene::createScene()));
        }
    });
    this->addChild(return_button, 1);

    return true;
}


void NetMenu::menuCloseCallback(Ref* pSender)
{

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void NetMenu::textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type)
{
    switch (type)
    {
        case cocos2d::ui::TextField::EventType::ATTACH_WITH_IME:
        {
            cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
            Size screenSize = CCDirector::getInstance()->getWinSize();
            
            _pleaseStartButton->setVisible(true);
        }
            break;
            
        case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
        {
            cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);

            _playerName = textField->getString();
        }
            break;
            
        case cocos2d::ui::TextField::EventType::INSERT_TEXT:
        {
            TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
            _pleaseStartButton->setVisible(false);
            _playerName = textField->getString();
            _nameStartButton->setVisible(true);
            
        }
            break;
            
        case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
        {
            TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);

            _playerName = textField->getString();
        }
            break;
            
        default:
            break;
    }
}

void NetMenu::editBoxEditingDidBegin(EditBox* editBox){
    
}

void NetMenu::editBoxEditingDidEnd(EditBox* editBox){
    
}

void NetMenu::editBoxTextChanged(EditBox* editBox, const std::string& text){
    
}

void NetMenu::editBoxReturn(EditBox* editBox){
    
}
void NetMenu::initIPBox()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	IPBG = Sprite::create("LoginScene/input_normal.png");
	IPBG->setScale(1.2f);
	IPBG->setOpacity(200);

	IPInput = ui::TextField::create("Input IP Here", Settings::Font::Type::base, Settings::Font::Size::light);
	IPInput->setString(UserDefault::getInstance()->getStringForKey("IP", ""));
	IPInput->setColor(Color3B::BLACK);
	IPInput->setCursorChar('|');
	IPInput->setCursorEnabled(true);
	IPInput->setMaxLength(15);
	IPInput->setMaxLengthEnabled(true);
	IPInput->setPosition(IPBG->getContentSize() / 2);
	IPInput->addEventListener(CC_CALLBACK_2(NetMenu::textFieldEvent2, this));

	IPBG->addChild(IPInput);

	IPBG->setScale(1.5);
	IPBG->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height / 2 + IPBG->getContentSize().height * 2));

	addChild(IPBG);

}
void NetMenu::textFieldEvent2(Ref* sender, ui::TextField::EventType event)
{
	switch (event) {
	case ui::TextField::EventType::ATTACH_WITH_IME:
		IPBG->setTexture("LoginScene/input_active.png");
		break;
	case ui::TextField::EventType::DETACH_WITH_IME:
		IPBG->setTexture("LoginScene/input_normal.png");
		break;
	}
}


