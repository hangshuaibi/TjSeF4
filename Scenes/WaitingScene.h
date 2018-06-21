#pragma once

/*玩家在此场景输入ip地址，若连接失败则需要重新输入ip
否则等待服务端开始游戏即自动切到主场景*/

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class WaitingScene :public Scene {
public:
	CREATE_FUNC(WaitingScene);

private:
	bool init()override;

	void update(float delta)override;
};
