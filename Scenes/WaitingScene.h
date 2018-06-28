#pragma once

/*玩家在此场景可以搜索局域网下的房间列表并选择加
入，或者输入ip地址，若连接失败则需要重新输入ip连接
成功则等待服务端开始游戏即自动切到主场景*/

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class WaitingScene :public Scene {
public:
	CREATE_FUNC(WaitingScene);

private:
	enum Notice {//提示消息类型定义
		CONNECT_TRY = 0,//尝试连接，请等待
		CONNECT_SUCCESS,//连接成功，等待主机开始游戏
		CONNET_FAIL,//连接失败，重新输入
		VALID_IP,//ip不合法，重新输入
	};

	bool init()override;

	void update(float delta)override;

	void notice(Notice type);//消息提示

	void EnterHandle(Ref* pSender, Widget::TouchEventType type);

	TextField* _inputIpWindow = nullptr;

	bool isIpValid(const std::string& ip);//检验ip是否合法

	bool isPartOfIp(const std::string& str);//检验str是否为合法ip地址的一部分

	void refresh(int mode);
};
