#pragma once

/*这是互联网对战的房间界面，当人数大于等于两个人并且同时准备即稍后开始游戏*/

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Network/Client.h"
#include "MainScene.h"


class NetScene :public cocos2d::Scene {
public:
	CREATE_FUNC(NetScene);

private:
	Client * _client = nullptr;

	bool init()override
	{
		if (!Scene::init())
		{
			return false;
		}
		_client = Client::create("118.25.134.24");
		this->addChild(_client);
		Sleep(1000);

		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

		auto background = Sprite::create("scene/netscene.jpg");
		this->addChild(background);
		background->setAnchorPoint(cocos2d::Vec2::ZERO);

		auto enterButton = cocos2d::ui::Button::create("scene/enter.png");
		enterButton->setScale(0.6f);
		this->addChild(enterButton);
		enterButton->setPosition(cocos2d::Vec2(visibleSize.width * 0.5f, visibleSize.height / 2));
		//enterButton->setTitleText("enter");
		enterButton->setTitleFontSize(7);
		enterButton->addTouchEventListener([=](Ref* pSender, cocos2d::ui::Widget::TouchEventType type) {
			if (cocos2d::ui::Widget::TouchEventType::ENDED == type)
			{
				_client->sendMessage("Client is ready");
			}
		});

		scheduleUpdate();

		return true;
	}

	void update(float delta)
	{
		if (_client->isStart())
		{
			auto scene = TransitionFadeDown::create(2.0f, MainScene::create(_client));
			Director::getInstance()->pushScene(scene);
		}
	}
};