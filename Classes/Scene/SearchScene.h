﻿/*****************************************************************************
*  Copyright (C) 2017 李坤 1061152718@qq.com
*
*  此文件属于软件学院2017c++大项目泡泡堂选题的项目文件.
*
*  此项目是开源项目, 在期末答辩之后, 我们可能会在假期里对一些因时间不够未完成的功能进
*  行补充, 以及适配windows平台, 在未来如果技术允许的情况下, 会酌情开发ios版本和anroid
*  版本, 期待您能够为这个开源项目提供宝贵意见, 帮助我们做得更好, 如果能够贡献宝贵的代
*  码那就更令人开心了.
*
*  本项目遵守MIT开源协议, 这也就是说, 您需要遵守的唯一条件就是在修改后的代码或者发行
*  包包含原作者的许可信息. 除非获得原作者的特殊许可, 在任何项目(包括商业项目)里使用
*  本项目的文件都需要包含作者的许可.
*
*  如果对项目有疑问或者其他建议, 欢迎联系13167211978@163.com, 1061152718@qq.com,
*  我们期待能和您互相交流合作, 学习更多的知识.
*
*  另外注意: 此项目需要您自行配置cocos环境,安装boost库, 如果遇到相关问题的话, 欢迎将
*  错误日志发给我们, 您的帮助将有助于改善游戏的体验.
*
*  @file     SearchScene.h
*  @brief    选择加入游戏时进入该场景，可搜索并显示房间列表
*
*
*  @author   宋天佑 李坤
*  @email    1061152718@qq.com
*  @version  4.0.1.5(版本号)
*  @date     2017/06/06
*  @license  Massachusetts Institute of Technology License (MIT)
*
*----------------------------------------------------------------------------
*  Remark         : Description
*----------------------------------------------------------------------------
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*----------------------------------------------------------------------------
*  2017/06/06 | 4.0.0.1   | 宋天佑            | Create file
*----------------------------------------------------------------------------
*
*****************************************************************************/

#ifndef SearchScene_h
#define SearchScene_h

#include <iostream>
#include "cocos2d.h"
#include "json/document.h"
#include "../Data/LevelData.h"
#include "GameScene.h"
#include "../NetWork/LocalServer.h"
#include "../NetWork/Client.h"

/**
*   @brief  搜索界面\n
*           搜索并显示房间列表，可点击connect并切换到房间场景
*/
class SearchScene : public cocos2d::Layer
{
private:

	int                 _selectLevelIndex;      ///地图索引
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
