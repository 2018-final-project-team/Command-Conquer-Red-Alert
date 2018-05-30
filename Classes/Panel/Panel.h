/*
*  @file     Panel.h
*  @brief    控制栏类，使用控制栏进行建造建筑、训练士兵、制造战车
*  @author   王亮
*/

#ifndef _PANEL_H_
#define _PANEL_H_

#include "cocos2d.h"

typedef enum
{
	BUILDING,
	SOLDIER,
	CAR
}Category;

class Panel : public cocos2d::Sprite
{
private:
	//Sprite* _panelBG;
	Category _curCategory;
	cocos2d::Vector<cocos2d::Sprite*> _buildingList;
	cocos2d::Vector<cocos2d::Sprite*> _soldierList;
	cocos2d::Vector<cocos2d::Sprite*> _carList;


	void addIcons();
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

    
public:
	virtual bool init() override;
	CREATE_FUNC(Panel);
      
};

#endif