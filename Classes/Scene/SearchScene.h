
#ifndef SearchScene_h
#define SearchScene_h

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"
#include "Data/LevelData.h"
#include "GameScene.h"
#include "NetWork/LocalServer.h"
#include "NetWork/Client.h"

/**
*   @brief  搜索界面\n
*           搜索并显示房间列表，可点击connect并切换到房间场景
*/
class SearchScene : public cocos2d::Layer
{
private:

	int                 selectMapIndex;      ///地图索引
	Sprite*             _room_list_bg;          ///房间列表的背景图片名称
	std::string         _player_name;           ///玩家名称
	Vector<Button*>     button_list;            ///存储所列出的房间对象

public:
	/**
	* @brief    创建房间列表场景
	*/
	static cocos2d::Scene* createScene(Client* client, std::string _playerName);

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
	void menuCloseCallback(cocos2d::Ref* pSender);

	/**
	* @brief    每帧都调用该函数
	*
	* @param    delta           update的默认参数
	*
	*/
	void update(float delta);

	/**
	* @brief    在create()后调用init(), 并让相应对象加入cocos自动内存管理系统
	*/
	CREATE_FUNC(SearchScene);
};

#endif /* SearchScene_h */
