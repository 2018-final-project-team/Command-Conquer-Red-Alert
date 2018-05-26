//
//  UnitData.hpp
//  CCRA
//
//  Created by 王星洲 on 2018/5/23.
//

#ifndef UnitData_hpp
#define UnitData_hpp

#include "cocos2d.h"
#include "string"
#include "Scene/GameScene.h"
USING_NS_CC;
//TODO: 加入动画和图片素材，之后实现。

namespace unitData
{
    const int infantryCastMoney = 0;
    const int infantryCastPower = 0;
    const int infantryWait = 0;
    const int dogCastMoney = 0;
    const int dogCastPower = 0;
    const int dogWait = 0;
    const int tankCastMoney = 0;
    const int tankCastPower = 0;
    const int tankWait = 0;
}

//单位状态
typedef enum
{
    //默认状态
    UNIT_DEFAULT = 0,
    //守备状态
    UNIT_DEFENCE = 1,
    //移动状态
    UNIT_MOVE = 2,
    //攻击状态
    UNIT_ATTACK = 3,
    //受伤状态
    UNIT_INJURE = 4,
    //死亡状态
    UNIT_DEAD =5
}UNIT_STATE;



class Unit : public Sprite{
public:
    //生成宏
    CREATE_FUNC(Unit);
    virtual bool init();
public:
    //构造函数
    Unit();
    
    //单位tag（步兵，狗，矿车，坦克）
    CC_SYNTHESIZE(Tag, _tag, UnitTag);
    
    //单位血量
    CC_SYNTHESIZE(int, _HP, UnitHP);
    
    //单位攻击力
    CC_SYNTHESIZE(int, _ATK, UnitATK);
    
    //单位速度
    CC_SYNTHESIZE(int, _Speed, UnitSpeed);
    
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

    //用于转向的第二方向
    CC_SYNTHESIZE(cocos2d::Vec2, _secondDirection, SecondDirection);

    //是否到达目的地
    CC_SYNTHESIZE(bool, _getDestination, GetDestination);

    //是否被选中
    CC_SYNTHESIZE(bool, _isSelected, IsSelected);
    
//    //动画名字
//    CC_SYNTHESIZE(std::string, _AnimationName, UnitAnimationName);
//
//    //动画路径
//    CC_SYNTHESIZE(std::string, _AdressName, _AdressName);
    
    //单位名字
    CC_SYNTHESIZE(std::string, _UnitName, _UnitName);

    //moveTo
    void moveTo(cocos2d::Vec2 destination)
    {
        //To Do:
    }
    
    //生成单位的方法
    static Unit * create(Tag _tag);
};



//状态管理类
class BaseFSM : public Ref
{
public:
    //创建状态
    static BaseFSM * createFSM(Unit * unit);
    //初始化
    bool init(Unit *unit);
public:
    //预留接口
    void switchState(int dir);
    
    //改变状态为默认状态
    void changeToDefault();
    
    //改变状态为攻击状态
    void changeToAttack();
    
    //改变朝向
    void changeToUp();
    void changeToLeftUp();
    void changeToLeft();
    void changeToLeftDown();
    void changeToDown();
    void changeToRightDown();
    void changeToRight();
    void changeToRightUp();
    
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
