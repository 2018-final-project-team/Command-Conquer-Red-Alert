#ifndef _LevelData_h_
#define _LevelData_h_

#include "cocos2d.h"
#include <iostream>
#include "ExternData.h"

USING_NS_CC;

/**
    * @brief 存储开始游戏的时候需要传入的数据 \n
    * 包括地图索引、背景图片名称、人物图片名称、房间内参与游戏的人物信息
*/

class LevelData : public Node
{
protected:

	int _mapIndex; 			/// 地图对应的索引

	std::string 	_bg; 	/// 地图缩略图名称
	std::string 	_data; 	/// 地图对应.data文件的名称
	std::string 	_role; 	/// 人物对应的图片名称

public:

    std::vector<PlayerData> player_list;	///房间内现存的玩家数据

	/** 
    * @brief 			单例对象创建函数
    * 
    * @param number    	地图索引
    * @param bg 		地图对应缩略图的名称
    * @param data		人物缩略图的名称
    * @param role 		人物角色名称
    *
    * @return 			指向LevelData对象的指针
	*/
	static LevelData* create(int number, std::string bg, std::string data, std::string role)
	{
		LevelData *sprite = new LevelData();
		if (sprite)
		{
			sprite->autorelease();
			sprite->setmapIndex(number);
			sprite->setBg(bg);
			sprite->setData(data);
			sprite->setRole(role);

			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	/** 
    * @brief 			设置地图对应的序号
    * @param number    	地图索引序号
	*/
	void setmapIndex(int number) { _mapIndex = number; }

	/** 
    * @brief            获得地图对应的序号
    * @return           地图索引序号
	*/
	int getmapIndex() { return _mapIndex; }

	/** 
    * @brief 			设置背景图片对应的名称
    * @param bg 	   	背景图片对应的名称
	*/
	void setBg(std::string bg) { _bg = bg; }

	/** 
    * @brief            获得背景图片对应的名称
    * @return           背景图片对应的名称
	*/
	std::string getBg() { return _bg; }
    
	/** 
    * @brief 			设置人物缩略图的名称
    * @param data    	背景图片对应的名称
	*/
	void setData(std::string data) { _data = data; }

	/** 
    * @brief            获得人物缩略图的名称
    * @return           背景图片对应的名称
	*/
	std::string getData() { return _data; }

	/** 
    * @brief 			设置人物角色名称
    * @param role   	人物角色名称
	*/
	void setRole(std::string role) { _role = role; }

	/** 
    * @brief            获得人物角色名称
    * @return           人物角色名称
	*/
	std::string getRole() { return _role; }

};

#endif /* _LevelData_h_ */
