/*
*  @file     NetMenu.h
*  @brief    开始游戏到进入房间方式的选择
*  @brief    功能：选择两种连接方式，选择创建或加入房间
*  @author   wxz
*/

#ifndef _NetMenu_h_
#define _NetMenu_h_
#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "NetWork/LocalServer.h"
#include "Scene/RoomScene.h"
#include "NetWork/Client.h"

USING_NS_CC;
using namespace ui;
/**
    * @brief 网络选择界面 \n
    * 在此选择局域网对战或互联网对战，并启动相应的local server和client\n
    * 在局域网模式下, 只有选择创建房间时才会同时启动client和server, 否则只启动client\n
    * 在互联网模式下, 只启动client
*/
class NetMenu : public cocos2d::Layer,public EditBoxDelegate
{
protected:
    int             selectMapIndex;      ///地图对应的索引

    std::string     _playerName;            ///玩家名称

    Button *        _nameStartButton;       ///指向输入框的指针
    Button *        _pleaseStartButton;     ///指向确认输入按钮的指针

	cocos2d::Sprite*               IPBG;
	cocos2d::ui::TextField*        IPInput;
	std::string                    IP;		//房主IP
public:
    /** 
    * @brief    单例对象创建函数
    *
    * @return   指向网络菜单场景的指针
    */
	static cocos2d::Scene* createScene(std::string playerName);

    /** 
    * @brief    自定义的初始化函数
    *
    * @return   初始化的结果
    *        -<em>false</em>    初始化失败
    *        -<em>true</em>     初始化成功
    */
    virtual bool init();
    
    /** 
    * @brief    关闭游戏的回调函数
    * 
    * @param    psender         指针, 指向调用回调函数的CCObject
    *
    */
    void menuCloseCallback(Ref* pSender);
    
    /**
    * @brief    在create()后调用init(), 并让相应对象加入cocos自动内存管理系统
    */
    CREATE_FUNC(NetMenu);



    /** 
    * @brief    当动作开始的时候调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    */
    void editBoxEditingDidBegin(EditBox* editBox);

    /** 
    * @brief    当离开输入框的时候调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    */
    void editBoxEditingDidEnd(EditBox* editBox);

    /** 
    * @brief    当输入框中内容被改变的时候调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    * @param    text            输入的字符串
    */
    void editBoxTextChanged(EditBox* editBox, const std::string& text);

    /** 
    * @brief    按下回车时调用的函数
    * 
    * @param    editBox         指针，指向被操作的输入框对象
    */
    void editBoxReturn(EditBox* editBox);

	/**
	* @brief    弹出输入IP的窗口
	*/
	void initIPBox();

	/**
	* @brief    记录IP信息
	*/
	void textFieldEvent2(Ref* sender, ui::TextField::EventType event);

	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
};

#endif /* _NetMenu_h_ */
