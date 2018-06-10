/*
*  @file     SettingsScene.h
*  @brief    设置场景类，可设置音乐、音效的打开/关闭
*  @author   王亮
*/

#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SettingsScene : public cocos2d::Layer
{
public:
	cocos2d::EventListenerKeyboard* keyEventListener;

private:
    void menuOkCallback(cocos2d::Ref *pSender);

    cocos2d::ui::CheckBox * createCheckBox(std::function<void(cocos2d::Ref*, cocos2d::ui::CheckBox::EventType)> callback);
	cocos2d::Label * createLabel(const char * text);

	/**
	@brief 用内容为“OK”的label创建一个menu，用于从设置场景返回主场景
	@return cocos2d::Menu*
	*/
    cocos2d::Menu* createText();

	void onExit();
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(SettingsScene);
};

#endif // _SETTINGS_SCENE_H__