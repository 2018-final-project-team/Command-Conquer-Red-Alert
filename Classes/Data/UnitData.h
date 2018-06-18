//
//  UnitData.hpp
//

#ifndef UnitData_hpp
#define UnitData_hpp

#include "cocos2d.h"
#include "string"
#include "TagData.h"


//TODO: 加入动画和图片素材，之后实现。
//TODO: 添加注释。

namespace unitData
{
    const int infantryCostMoney = 50;
    const int dogCostMoney = 50;
    const int tankCostMoney = 200;
    
    namespace EnoughPower
    {
        const int infantryWait = 3 * 1000;
        const int dogWait = 3 * 1000;
        const int tankWait = 6 * 1000;
    }
    
    namespace NotEnoughPower
    {
        const int infantryWait = 6 * 1000;
        const int dogWait = 6 * 1000;
        const int tankWait = 12 * 1000;
    }
}

typedef enum {
	stateNone = 0, //无状态
	stateWalkRight, //向右走
	stateWalkLeft, //向左走
	stateWalkUp, //向上走
	stateWalkDown, //向下走
	stateAttackLeft, //向左攻击
	stateAttackRight, //向右攻击
	stateDeath,//死亡
}UnitState;


////单位状态
//typedef enum
//{
//    //默认状态
//    UNIT_DEFAULT = 0,
//    //守备状态
//    UNIT_DEFENCE = 1,
//    //移动状态
//    UNIT_MOVE = 2,
//    //攻击状态
//    UNIT_ATTACK = 3,
//    //受伤状态
//    UNIT_INJURE = 4,
//    //死亡状态
//    UNIT_DEAD = 5
//}UNIT_STATE;

class Building;

class Unit : public cocos2d::Sprite{

public:
	cocos2d::Sprite* _bloodBox;              //血槽Sprite
	cocos2d::ProgressTimer* _bloodBarPt;     //血条ProgressTimer
	cocos2d::Sprite* _ring;                  //被选中时脚下的光环

	//最后一次动作状态，用于更新默认状态的静态图片
	CC_SYNTHESIZE(UnitState, _lastTurn, LastTurn);
   
    //是否被选中
	//CC_SYNTHESIZE(bool, _isSelected, IsSelected);
	bool _isSelected;
	bool getIsSelected();
	void setIsSelected(bool b);
    
    //单位tag（步兵，狗，矿车，坦克）
    CC_SYNTHESIZE(Tag, _unitTag, UnitTag);
    
    //当前血量
    CC_SYNTHESIZE(int, _HP, UnitHP);

	//满血血量
	CC_SYNTHESIZE(int, _FullHP, UnitFullHP);
    
    //单位攻击力
    CC_SYNTHESIZE(int, _ATK, UnitATK);
    
    //单位攻击间隔
    CC_SYNTHESIZE(float, _ATKCD, UnitATKCD);
    
    //单位速度
    CC_SYNTHESIZE(float, _Speed, UnitSpeed);
    
    //单位所需金钱
    CC_SYNTHESIZE(int, _Value, UnitValue);
    
    //单位所需准备时间
    CC_SYNTHESIZE(int, _CD,UnitCD);
    
    //锁敌区域
    CC_SYNTHESIZE(unsigned int, _LockLimit, UnitLockLimit);
    
    //攻击区域
    CC_SYNTHESIZE(unsigned int, _ATKLimit, UnitATKLimit);
    
    //目的地
    CC_SYNTHESIZE(cocos2d::Vec2, _destination, Destination);
    
    //是否抵达目的地
    CC_SYNTHESIZE(bool, _getDestination, GetDestination);
    
    //单位名字
    CC_SYNTHESIZE(std::string, _UnitName, UnitName);

	//单位状态
	CC_SYNTHESIZE(UnitState, _UnitState, UnitState );
    
    //生成单位的方法
    static Unit * create(Tag unitTag);

    // 用于寻路
    std::vector<cocos2d::Point> _route;
    
public:
    /*
     */
    void moveTo(cocos2d::Vec2 destination);
    /*
     */
    void getInjuredBy(Unit *);

	void decreaseHP(int num);
    /*
     */
    void attack(Unit *);
    /*
     */
    void attack(Building *);
    /*
     */
    bool canAttack(cocos2d::Vec2);
    /*
     */
    void setDeath()
    {
        // 也许以后有用
    }
    
    //状态管理

public:
    //改变状态的接口
    void switchState(UnitState state);
    
    //改变状态为默认状态
    void changeToDefault();
    
    //改变状态为攻击状态-向左
    void changeToAttackLeft();

	//改变状态为攻击状态-向右
	void changeToAttackRight();
    
    //改变朝向
    void changeToUp();
    
    void changeToLeft();
    
    void changeToDown();
    
    void changeToRight();
    
    
    //改变状态到守备
    void changeToDefence();
    
    //改变状态为受伤状态
    void changeToInjure();
    
    //改变状态为死亡
    void changeToDead();
    
    //发现敌人后向敌人移动
    void changeToEnemy();
    
    
};





#endif /* UnitData_hpp */
