/*
*  @file     LoadingScene.h
*  @brief    鍔犺浇鍦烘櫙绫伙紝鏄剧ず鐨勭涓€涓満鏅紝瀹屾垚璧勬簮棰勫姞杞藉苟鏄剧ず鍔犺浇杩涘害
*  @author   鐜嬩寒
*/

#ifndef __SCENE_LOADINGSCENE_H__
#define __SCENE_LOADINGSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LoadingScene : public cocos2d::Layer
{
private:
	cocos2d::ui::LoadingBar* loadingBar;

private:

	/**
	@brief 璧勬簮鍏ㄩ儴鍔犺浇缁撴潫鍚庝緵scheduleOnce璋冪敤鐨勫嚱鏁帮紝寤舵椂dt璺宠浆鑷崇櫥褰曞満鏅?
	@param float dt 鏃堕棿闂撮殧锛屽崟浣嶄负绉?
	*/
	void endLoading(float dt);

	/**
	@brief 鍔犺浇杩囩▼锛岃繘琛岃祫婧愮殑鍔犺浇鍜岃繘搴︽潯鐨勬洿鏂?
	*/
	void loading();

	/**
	@brief 鍔犺浇鍚勭被璧勬簮
	*/
	void loadSound();
	void loadImage();
	void loadSpriteFrame();
	void loadAnimation();


	void initBackground(cocos2d::Vec2 origin, cocos2d::Size visibleSize);
	cocos2d::ui::LoadingBar* createLoadingBar();
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LoadingScene);
};

#endif // __SCENE_LOADINGSCENE_H__