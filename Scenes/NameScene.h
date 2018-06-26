#pragma once

/*这是一个获取名字的场景，现在主要是为了实现服务端（可以认为是
房间）有名字的逻辑，想法是把名字作为全局变量传啊传，当然也可以
直接在create server的把名字作为class内部变量，考虑如此有两个原因，
1代码改动不大 2一般而言一台电脑只作为一个服务端或者客户端，这也
可以实现房间内玩家的名称显示功能。当然主要是因为1（逃*/

#include "cocos2d.h"

class NameScene :public cocos2d::Scene {
public:
	CREATE_FUNC(NameScene);

	static std::string getName();
private:
	bool init()override;
};