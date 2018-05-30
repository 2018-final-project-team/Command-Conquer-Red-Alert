/*
*  @file     GameAudio.cpp
*  @brief    声音控制工具类，提供控制游戏的音乐和音效的方法
*  @author   王亮
*/


#include "GameAudio.h"

USING_NS_CC;

#define SET_KEY(name, val) UserDefault::getInstance()->setBoolForKey(name, val)

GameAudio::GameAudio()
{
	audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	isEffectOn = cocos2d::UserDefault::getInstance()->getBoolForKey("isEffectOn", true);
	isBgmOn = cocos2d::UserDefault::getInstance()->getBoolForKey("isBgmOn", true);
};


GameAudio * GameAudio::getInstance()
{
    static GameAudio INSTANCE;
    return &INSTANCE;
}

void GameAudio::preloadBgm(const char * pszFilePath)
{
	audioEngine->preloadBackgroundMusic(pszFilePath);
}

void GameAudio::preloadEffect(const char * pszFilePath)
{
	audioEngine->preloadEffect(pszFilePath);
}


void GameAudio::playEffect(const char * name)
{
    if (isEffectOn)
    {
        audioEngine->playEffect(name);
    }
}

void GameAudio::stopEffect()
{
    audioEngine->stopAllEffects();
}

void GameAudio::setEffectOn(bool val)
{
    isEffectOn = val;
    SET_KEY("isEffectOn", val);
    if (val)
    {
        audioEngine->resumeAllEffects();
    }
    else
    {
        stopEffect();
    }
}

bool GameAudio::getEffectOn()
{
    return isEffectOn;
}

void GameAudio::playBgm(const char * name)
{
    if (currentBgm != name)
    {
        currentBgm = name;
        audioEngine->playBackgroundMusic(name, true);
    }
    if (!isBgmOn)
    {
        stopBgm();
    }
}

void GameAudio::stopBgm()
{
    audioEngine->stopBackgroundMusic();
}

void GameAudio::setBgmOn(bool val)
{
    isBgmOn = val;
    SET_KEY("isBgmOn", val);
    if (val)
    {
        audioEngine->resumeBackgroundMusic();
    }
    else
    {
		audioEngine->pauseBackgroundMusic();
    }
}

bool GameAudio::getBgmOn()
{
    return isBgmOn;
}


