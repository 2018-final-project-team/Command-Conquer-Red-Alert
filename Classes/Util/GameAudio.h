/*
*  @file     GameAudio.h
*  @brief    �������ƹ����࣬�ṩ������Ϸ�����ֺ���Ч�ķ���
*  @author   ����
*/

#ifndef __UTIL_GAMEAUDIO_H__
#define __UTIL_GAMEAUDIO_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class GameAudio
{
private:

	bool isEffectOn;               //��Ч�Ƿ��
	bool isBgmOn;                  //�����Ƿ��

	std::string currentBgm;
	CocosDenshion::SimpleAudioEngine * audioEngine;       //��Ƶ����

public:

    static GameAudio * getInstance();

	void preloadBgm(const char * pszFilePath);

	void preloadEffect(const char * pszFilePath);

    void playEffect(const char * name);

    void stopEffect();

    void setEffectOn(bool val);

    bool getEffectOn();

    void playBgm(const char * name);

    void stopBgm();

    void setBgmOn(bool val);

    bool getBgmOn();
private:
	//���캯��
	GameAudio();

};


#endif