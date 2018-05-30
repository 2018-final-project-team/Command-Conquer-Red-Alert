/*
*  @file     preloadList.h
*  @brief    ��ҪԤ���ص���Դ���ļ�������·����
*  @author   ����
*/

#ifndef __PRELOAD_LIST_H__
#define __PRELOAD_LIST_H__

#include <array>

namespace Preload
{
	//ͼ����Ӧ��plist�ļ����ַ�������
    const std::array<std::string, 2> plists = {
        "Animation/explosion.plist",
        "Animation/wind.plist"
    };

	//ͼƬ�ļ����ַ�������
	const std::array<std::string, 0> image = {

	};

	//���������ļ����ַ�������
	const std::array<std::string, 1> bgm = {
		"Sound/bgm/WelcomeScene.mp3",
	};

	//��Ч�ļ����ַ�������
	const std::array<std::string, 0> effect = {
		
	};
}

#endif // !__PRELOAD_LIST_H__

