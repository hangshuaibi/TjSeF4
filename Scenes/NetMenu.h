#pragma once

/*NetMenu是开始菜单类，在这里你可以选
择局域网对战和互联网对战*/

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ServerOrNot.h"

//USING_NS_CC;
//using namespace cocos2d::ui;

class NetMenu :public cocos2d::Scene {
public:
	CREATE_FUNC(NetMenu);

private:
	

	bool init()override
	{
		if (!Scene::init())
		{
			return false;
		}
		auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
		//背景
		auto background = cocos2d::Sprite::create("scene/bg.jpg");
		assert(background != nullptr);

		//background->setScaleX(2);
		//background->setScaleY(3);
		this->addChild(background);
		background->setAnchorPoint(cocos2d::Vec2::ZERO);

		//互联网
		auto netButton = cocos2d::ui::Button::create("scene/netfight.png");
		this->addChild(netButton);
		netButton->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height * 0.9));
		netButton->setTouchEnabled(true);

		netButton->addTouchEventListener([&](Ref* pSender, cocos2d::ui::Widget::TouchEventType type) {

			if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {

			}
		});

		// 局域网
		auto lanButton = cocos2d::ui::Button::create("scene/lanfight.png");
		this->addChild(lanButton);
		lanButton->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height * 0.7));
		lanButton->setTouchEnabled(true);

		lanButton->addTouchEventListener([&](Ref* pSender, cocos2d::ui::Widget::TouchEventType type) {
			if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
				auto transiton = TransitionSlideInL::create(2.f, ServerOrNot::create());
				Director::getInstance()->replaceScene(transiton);
			}
		});


		// “游戏设置”按钮
		auto setButton = cocos2d::ui::Button::create("scene/settings.png");
		setButton->setTitleFontName("微软雅黑");
		setButton->setTitleFontSize(12);
		setButton->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height*0.5));
		setButton->addTouchEventListener([](Ref* pSender, cocos2d::ui::Widget::TouchEventType type) {
			if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
				// 切换到AudioControl场景
			}
		});

		this->addChild(setButton);


		// “退出游戏”按钮
		auto closeButton = cocos2d::ui::Button::create("scene/quit.png");
		closeButton->setTitleFontName("微软雅黑");
		closeButton->setTitleFontSize(12);
		closeButton->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height*0.3));

		closeButton->addTouchEventListener([](Ref* pSender, cocos2d::ui::Widget::TouchEventType type) {
			if (type == cocos2d::ui::Widget::TouchEventType::ENDED) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
				MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
				return;
#endif
				cocos2d::Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
#endif
			}
		});
		this->addChild(closeButton);

		return true;
	}//init

};
