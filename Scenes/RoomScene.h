#pragma once

/*这是作为服务端后切换后的场景，可根据等待玩家的数量选择是否开始游戏
在此场景中需要初始化客户端和服务端*/

#include "cocos2d.h"
#include "Network/Client.h"
#include "Network/LocalServer.h"
#include <string>

using std::string;
USING_NS_CC;

class RoomScene :public Scene {
public:
	CREATE_FUNC(RoomScene);

	//更新玩家人数
	void update(float delta)override;
private:
	bool init()override;

	LocalServer* _server = nullptr;
	Client* _client = nullptr;

	Label* _clientNum = nullptr;//显示玩家数
	Label* _readyClientNum = nullptr;//显示已经准备好的玩家数

	const string num2Str(int num);
};