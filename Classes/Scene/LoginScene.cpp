/*
*  @file     LoginScene.cpp
*  @brief    登录场景类，显示的第二个场景，输入并持久化保存玩家id
*  @author   王亮
*/

#include "LoginScene.h"
#include "Settings.h"
#include "WelcomeScene.h"
#include "Util/GameAudio.h"

USING_NS_CC;

Scene* LoginScene::createScene()
{
    auto scene = Scene::create();

    auto layer = LoginScene::create();

    scene->addChild(layer);

    return scene;
}

bool LoginScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    createBackground();
    createLoginButton();

    initUserBox();

    return true;
}

void LoginScene::createBackground()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite * bg = Sprite::create("background.png");
    //set transparent
    bg->setOpacity(255);
    bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

    addChild(bg);
}

void LoginScene::createLoginButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto loginButton = ui::Button::create("button_normal.png", "button_selected.png");
    loginButton->setTitleText("Login");
	loginButton->setTitleFontName(Settings::Font::Type::official);
    loginButton->setTitleFontSize(Settings::Font::Size::label);
    loginButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.35f));
    loginButton->setOpacity(233);


    loginButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
    {
        if (type == ui::Widget::TouchEventType::ENDED)
        {
            GameAudio::getInstance()->playEffect("Sound/button.wav");
            return;
        }
		username = usernameInput->getString();
        if (username.empty())
        {
            MessageBox("Username can't be empty", "Alert");
        }
        else
        {
            username.substr(0, 16);
            UserDefault::getInstance()->setStringForKey("username", username);

            Director::getInstance()->replaceScene(TransitionFade::create(1.2f, WelcomeScene::createScene(username)));
        }
    });
    addChild(loginButton);
}

void LoginScene::initUserBox()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    usernameBG = Sprite::create("LoginScene/input_normal.png");
    usernameBG->setScale(1.2f);
    usernameBG->setOpacity(200);

    usernameInput = ui::TextField::create("Input Username Here", Settings::Font::Type::base, Settings::Font::Size::light);
    usernameInput->setString(UserDefault::getInstance()->getStringForKey("username", ""));
    usernameInput->setColor(Color3B::BLACK);
    usernameInput->setCursorChar('|');
    usernameInput->setCursorEnabled(true);
    usernameInput->setMaxLength(15);
    usernameInput->setMaxLengthEnabled(true);
    usernameInput->setPosition(usernameBG->getContentSize() / 2);
    usernameInput->addEventListener(CC_CALLBACK_2(LoginScene::textFieldEvent, this));

    usernameBG->addChild(usernameInput);

    usernameBG->setScale(1.5);
    usernameBG->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height / 2 + usernameBG->getContentSize().height * 2));

    addChild(usernameBG);

}

void LoginScene::textFieldEvent(Ref* sender, ui::TextField::EventType event)
{
    switch (event) {
    case ui::TextField::EventType::ATTACH_WITH_IME:
        usernameBG->setTexture("LoginScene/input_active.png");
        break;
    case ui::TextField::EventType::DETACH_WITH_IME:
        usernameBG->setTexture("LoginScene/input_normal.png");
        break;
    }
}

void LoginScene::onEnter()
{
    Layer::onEnter();
    GameAudio::getInstance()->playBgm("Sound/WelcomeScene.mp3");
}
