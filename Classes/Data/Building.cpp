//
//  BuildingData.cpp
//  CCRA
//
//  Created by 王星洲 on 2018/5/23.
//  Modified by 王亮 on 2018/5/23
//

#include "Building.h"
#include "Scene/GameScene.h"
#include "cocos2d.h"


using namespace cocos2d;


//创建建筑物时调用的方法
Building* Building::create(Tag _buildingTag)
{
    //创建一个建筑物精灵对象
    Building* temp = new Building();
    //添加物理
    auto body = PhysicsBody::createCircle(temp -> getContentSize().width/2);
    
    body->setContactTestBitmask(1);//设置为1，开启其他刚体与此物理刚体产生碰撞，设置它才能触发碰撞检测回调函数
    
	temp->setPhysicsBody(body);

    //定义一个存放建筑物的字符串数组
   std::string picTable[5] = { "GameItem/Building/Base200.png","GameItem/Building/Power200.png","GameItem/Building/Mine200.png","GameItem/Building/Barracks150.png","GameItem/Building/CarFactory200.png" };
    //初始化安装建筑物时金钱的消耗
    int sValue[5] = { 500,500,500,500,500 };
    //初始化建筑物的血量
    int sHP[5] = { 2000,500,500,500,500 };
    
    
    //初始化建筑物精灵对象
    temp->initWithFile(picTable[_buildingTag].c_str());
    //自动释放
    temp->autorelease();
    //拿到当前建筑物的_tag
    temp->_tag = _buildingTag;
    //设置血量
    temp->_hp = sHP[_buildingTag];
    //根据_tag设置安装各建筑物时需要的金钱
    temp->_value = sValue[_buildingTag];
    //卖掉建筑物时得到的金钱
    temp->_sellValue = temp->_value / 2;
    
    return temp;
}
