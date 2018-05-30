/*
*  @file     Panel.h
*  @brief    控制栏类，使用控制栏进行建造建筑、训练士兵、制造战车
*  @author   王亮
*/

#ifndef _PANEL_H_
#define _PANEL_H_

#include "cocos2d.h"
#include "Data/Building.h"

class Panel : public cocos2d::Sprite
{
private:
	cocos2d::Sprite* _panelBG;
	cocos2d::Sprite* _buildingButton;
	cocos2d::Sprite* _soldierButton;
	cocos2d::Sprite* _carButton;

	cocos2d::EventListenerTouchOneByOne* _mainButtonListener;


	CC_SYNTHESIZE(Tag, _curCategoryTag, CurCategoryTag);
	cocos2d::Vector<cocos2d::Sprite*> _buildingList;
	cocos2d::Vector<cocos2d::Sprite*> _soldierList;
	cocos2d::Vector<cocos2d::Sprite*> _carList;


	void addIcons();
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

	

  public:
	void setCurButton(Tag tag);

	virtual bool init() override;
	CREATE_FUNC(Panel);
      
};

#endif