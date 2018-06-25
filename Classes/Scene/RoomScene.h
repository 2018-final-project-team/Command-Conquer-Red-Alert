#ifndef _RoomScene_h_
#define _RoomScene_h_

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"
#include "Data/LevelData.h"
#include "Scene/GameScene.h"
#include "NetWork/LocalServer.h"
#include "NetWork/Client.h"
#include "Data/ExternData.h"
#include "Scene/NetMenu.h"
/**
* @brief 房间类,开房开房开房！继承自cocos2d:Layer
* 这里房间信息可以实时更新,玩家可以随便聊天
*/
class RoomScene : public cocos2d::Layer
{
protected:
	Vector<Sprite*>     _exist_player;             ///目前房间里的Player
	int                 selectMapIndex;         ///目前选择的地图索引
	Sprite*             _room_ptr;                 ///房间背景
	std::string         _owner_player_name;        ///房主的姓名
	PlayerData*         _owner_player_data;        ///房主的个人信息
	ui::Button*             _start_game_button;        ///开始游戏按钮
	std::string         _chatString;               ///聊天信息
	ui::TextField*          _chatWindow;               ///聊天输入框

    std::string     _playerName;            ///玩家名称

    ui::Button *        _nameStartButton;       ///指向输入框的指针
    ui::Button *        _pleaseStartButton;     ///指向确认输入按钮的指针
public:
	LevelData*          _game_data;                ///游戏信息

												   /**
												   * @brief                    Room的static create函数
												   * @param    c               客户端指针
												   * @param    mode            服务端or客户端
												   * @param    _playerName     玩家姓名
												   * @return   Scene*
												   */
	static cocos2d::Scene* createScene(Client* c, int mode, std::string _playerName);

	/**
	* @brief 做一些初始化的工作
	* @return  bool
	*/
	virtual bool init();
	/**
	* @brief 聊天输入框活动函数
	* @param type 活动的种类
	* @param pSender 信息传输指针
	* @return  void
	*/
	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	/**
	* @brief 初始化服务端
	* @return  void
	*/
	bool initForServer(void);
	/**
	* @brief 初始化客户端
	* @return  void
	*/
	bool initForClient(void);
	/**
	* @brief 找到玩家ID
	* @return  int
	*/
	int  findPlayerId();
	/**
	* @brief 每帧对房间进行更新
	* @return  void
	*/
	void update(float delta);

	CREATE_FUNC(RoomScene);
};

#endif /* _RoomScene_h_ */
