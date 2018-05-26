/*
*  @file     LoginScene.h
*  @brief    登录场景类，显示的第二个场景，输入并持久化保存玩家id
*  @author   王亮
*/

#ifndef  _LOGINSCENE_SCENE_H__
#define  _LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

class LoginScene :public cocos2d::Layer
{
private:

    cocos2d::Sprite* usernameBG;
    cocos2d::ui::TextField* usernameInput;

    void createBackground();
    void createLoginButton();

    void initUserBox();

    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType event);

    void onEnter();
public:

    static cocos2d::Scene * createScene();
    virtual bool init();
    CREATE_FUNC(LoginScene);

};
#endif // ! _LOGINSCENE_SCENE_H__
