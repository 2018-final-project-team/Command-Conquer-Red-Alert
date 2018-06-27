#ifndef _ExternData_h_
#define _ExternData_h_
#include <iostream>

/**
    * @brief 存储加入房间的玩家相关的数据 \n
    * 存储玩家的昵称, 玩家的角色, 玩家加入的顺序(即玩家的id)
*/

class PlayerData
{
public:
    std::string player_name;
    std::string player_role;
    int         player_id;
    
    PlayerData(std::string name, std::string role, int id) : player_name(name), player_role(role), player_id(id){}
    
};


#endif /* ExternData_h */

