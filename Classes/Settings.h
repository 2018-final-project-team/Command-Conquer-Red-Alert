#ifndef __SETTINGS_H__
#define __SETTINGS_H__


#include <cstdint>
#include <set>

namespace Settings
{
    namespace Font
    {
        namespace Size
        {
            constexpr int name = 18;
            constexpr int chat = 24;
            constexpr int title  = 60;
            constexpr int label  = 40;
            constexpr int normal = 32;
            constexpr int light  = 24;
            constexpr int result = 48;
        }
        namespace Type
        {
            constexpr char title[] = "fonts/Quicksand-Bold.ttf";
            constexpr char base[] = "fonts/OpenSans-Regular.ttf";
			constexpr char official[] = "fonts/Marker Felt.ttf";
        };
    }
    namespace Map
    {
        constexpr char path[] = "GameItem/Map/";
        static std::set<const char *> list{

        };
        namespace TileSize
        {
            constexpr int height = 40;
            constexpr int width = 40;
        }
    }
    namespace Player
    {
        constexpr char path[] = "GameItem/Player/";

    }
    namespace Building
    {
        constexpr char path[] = "GameItem/Building/";
    }
    namespace Soldier
    {
        constexpr char path[] = "GameItem/Soldier/";

    }
    //namespace Server
    //{
    //    constexpr char addr[] = "ws://115.159.188.148:4000";
    //}
}
#endif
