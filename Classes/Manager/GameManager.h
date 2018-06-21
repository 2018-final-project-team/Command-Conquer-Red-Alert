/**
*  @file     GameManager.h
*  @brief    游戏的控制类
**/

#ifndef __GameManager_H_
#define __GameManager_H_

#include "cocos2d.h"
#include "MoveController.h"
#include <queue>
#include <iostream>
#include <time.h>

class GameScene;              //解决头文件互相包含时带来的问题

class MoveController;         //解决头文件互相包含时带来的问题

class Panel;

class Manager : public cocos2d::Node
{
public:
	bool _isWaitToCreateBuilding;
	bool _canCreateBuilding;
	bool _isWaitToCreateSoldier;
	bool _isWaitToCreateCar;
private:

	cocos2d::Vec2   _destinationForMessage;   /// Unit的目的地
	std::string     _command;              /// 读取的远程信息
	int             _index;                /// 需要移动的Unit在对应Vector中的索引
	int             _playerId;             /// 人物Id
	Tag             _tagForMessage;        ///建造建筑或Unit的Tag
    cocos2d::Vec2   _positionForMessage;  ///建造建筑的位置
	Building*       _buildingForMessage;
	Unit*           _unitForMessage;


	Panel* _panel;
    GameScene* _gameScene;
    MoveController* _moveController;

    std::queue<Tag> _carQueue;               // 待建车队列
    std::queue<Tag> _soldierQueue;           // 待建士兵队列 


//========CreateController===========
	CC_SYNTHESIZE(clock_t, _timeToCreateBuilding, TimeToCreateBuilding);               // 单位毫秒
	CC_SYNTHESIZE(clock_t, _waitTimeToCreateBuilding, WaitTimeToCreateBuilding);
	CC_SYNTHESIZE(clock_t, _timeToCreateSoldier, TimeToCreateSoldier);
	CC_SYNTHESIZE(clock_t, _waitTimeToCreateSoldier, WaitTimeToCreateSoldier);
	CC_SYNTHESIZE(clock_t, _timeToCreateCar, TimeToCreateCar);
	CC_SYNTHESIZE(clock_t, _waitTimeToCreateCar, WaitTimeToCreateCar);

	CC_SYNTHESIZE(Tag, _buildingTag, BuildingTag);
	CC_SYNTHESIZE(Tag, _soldierTag, SoldierTag);
	CC_SYNTHESIZE(Tag, _carTag, CarTag);
    
//===================================

//==========Attack================
    Unit* _selectedEnemy;              //被选中的士兵或建筑
    Building* _selectedBuilding;

public:
    /**
    * @brief Manager的静态构造函数
    * @return  Manager*
    */
    static Manager* createWithGameScene(GameScene* gameScene);

    /**
    * @brief initWithGameScene
    * @return bool
    */
    bool initWithGameScene(GameScene* gameScene);

	/**
	* @brief 传递panel指针,在GameScene中被调用
	*/
	void setPanel(Panel* p);

    bool getCanCreateBuilding() { return _canCreateBuilding; }

//=====================CreateController========================
    /**
    * @brief 点击建造建筑的开始
    * @param1 建筑的Tag
    * @param2 开始等待的时间
    * @return  void
    */
    void clickCreateBuildingByTag(Tag building_tag, clock_t start_time);

    /**
    * @brief 点击建造士兵的开始
    * @param1 士兵的Tag
    * @return  void
    */
    void clickCreateSoldierByTag(Tag soldier_tag);

    /**
    * @brief 建造建筑的等待 需要在update函数里调用
    * @return  void
    */
    void waitCreateBuilding();

    /**
    * @brief 建造士兵的等待 需要在update函数里调用
    * @return  void
    */
    void waitCreateSoldier();

    /**
    * @brief 建造车的等待 需要在update函数里调用
    * @return  void
    */
    void waitCreateCar();

    /**
    * @brief 建造建筑 在GameScene中调用
    * @pamra 造建筑点
    * @return  void
    */
    void createBuilding(cocos2d::Vec2 position);

    /**
    * @brief 重新计算电量
    * @return  void
    */
    void resetPower();

//============================================================

//====================attack============================
    
    void setEnemy(Unit* enemy);
    void setBuilding(Building* building);

    /**
    * @brief 攻击 在Update函数里调用
    */
    void attack();

    /**
    * @brief 加钱 在Update函数里调用
    */
    void addMoneyUpdate();

    /*
    * @brief get _moveController
    * @return the  _movecontroller( a point )
    */
    MoveController* getMoveController()
    {
        return _moveController;
    }

    // 放士兵和坦克
    cocos2d::Point getPutSoldierPosition();

    cocos2d::Point getPutCarPosition();

    // 执行命令.
    void doCommands();

	//提取移动命令
	void readMoveCommand();

	//生成建造建筑命令
	std::string getCreateBuildingMessage(cocos2d::Vec2 pos, Tag tag);

	//提取建造建筑的命令
	void readCreateBuildingCommand();

	//生成创造Unit命令
	std::string getCreateUnitMessage(Tag tag, cocos2d::Vec2 pos);

	//提取创造Unit命令
	void readCreateUnitCommand();

	//生成移除建筑命令
	std::string Manager::getRemoveBuildingMessage(Building* b);

	//提取移除建筑命令
	void Manager::readRemoveBuildingCommand();

	//生成移除Unit命令
	std::string Manager::getRemoveUnitMessage(Unit* u);

	//提取移除Unit命令
	void Manager::readRemoveUnitCommand();

};

#endif // __GameManeger_H_
