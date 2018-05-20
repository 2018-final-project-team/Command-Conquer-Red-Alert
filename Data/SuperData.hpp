#ifndef Super_Data_hpp
#define Super_Data_hpp

#include "cocos2d.h"
USING_NS_CC;


/**
 * @brief 总类,继承自cocos2d:Sprite
 * 作为各具体单位建筑的父类。
 */
class Super : Sprite
{
private:
    std::string _name;
    int _life;
    int _powerNeed;
    int _goldNeed;
    bool _isExisting = false;
    bool _isSelected = false;
public:
    /*
     * @brief             创建函数
     *
     * @param number        地图索引
     * @param bg         地图对应缩略图的名称
     * @param data        人物缩略图的名称
     * @param role         人物角色名称
     *
     * @return             指向LevelData对象的指针
     */
    void moveTo();
};







#endif /* SuperData_hpp */
