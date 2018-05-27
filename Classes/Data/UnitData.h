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
//TODO: 添加注释。

namespace unitData
{
    const int infantryCastMoney = 50;
    const int infantryCastPower = 0;
    const int infantryWait = 10;
    const int dogCastMoney = 50;
    const int dogCastPower = 0;
    const int dogWait = 10;
    const int tankCastMoney = 200;
    const int tankCastPower = 0;
    const int tankWait = 30;
}


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



class Unit : public Sprite{
public:
    //生成宏
    CREATE_FUNC(Unit);
public:
    //构造函数
    Unit();
    
    //是否被选中
    CC_SYNTHESIZE(bool, _isSelected, UnitIsSelected)
    
    //单位tag（步兵，狗，矿车，坦克）
    CC_SYNTHESIZE(Tag, _tag, UnitTag);
    
    //单位血量
    CC_SYNTHESIZE(int, _HP, UnitHP);
    
    //单位攻击力
    CC_SYNTHESIZE(int, _ATK, UnitATK);
    
    //单位攻击间隔
    CC_SYNTHESIZE(float, _ATKCD, UnitATKCD);
    
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
    CC_SYNTHESIZE(Vec2, _destination, UnitDestination);
    
    //是否抵达目的地
    CC_SYNTHESIZE(bool, _getDestination, UnitGetDestination);
    
    
//    //动画名字
//    CC_SYNTHESIZE(std::string, _AnimationName, UnitAnimationName);
//
//    //动画路径
//    CC_SYNTHESIZE(std::string, _AdressName, _AdressName);
    
    //单位名字
    CC_SYNTHESIZE(std::string, _UnitName, _UnitName);
    
    //生成单位的方法
    static Unit * create(Tag _tag);
    
public:
    /*
     */
    bool setIsSelected(bool);
    /*
     */
    void moveTo(Vec2(destination),float time);
    /*
     */
    Vec2 getPosition();
    /*
     */
    Vec2 getDestination();
    /*
     */
    bool getGetDestination();
    /*
     */
    void setDestination(Vec2);
    /*
     */
    bool setGetDestination(bool);
    /*
     */
    void getInjuredBy(Unit *);
    /*
     */
    void attak(Unit *);
    /*
     */
    bool canAttack(Vec2);
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
