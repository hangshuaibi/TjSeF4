#include "NameScene.h"
#include "RoomScene.h"
#include <string>
#include <ui/CocosGUI.h>

using std::string;
USING_NS_CC;
using namespace ui;

string globalName;//全局变量，房间名/玩家名

bool NameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("scene/inputname.jpg");
	this->addChild(background);
	background->setAnchorPoint(cocos2d::Vec2::ZERO);

	//一个输入名字的窗口
	auto inputNameWindow = cocos2d::ui::TextField::create("  ", "Arial", 7);
	inputNameWindow->setMaxLengthEnabled(true);
	inputNameWindow->setMaxLength(20);
	inputNameWindow->setTouchSize(Size(200, 60));
	inputNameWindow->setTouchAreaEnabled(true);
	inputNameWindow->setPosition(Point(visibleSize.width / 3, visibleSize.height / 2));

	//窗口实体
	auto realWindow = Sprite::create("scene/InputBar.png");
	realWindow->setPosition(Point(visibleSize.width / 3, visibleSize.height / 2));

	this->addChild(inputNameWindow, 2);
	this->addChild(realWindow, 1);

	//回车键（
	auto enterButton = Button::create("scene/enter.png");
	enterButton->setScale(0.6f);
	this->addChild(enterButton);
	enterButton->setPosition(Point(visibleSize.width * 0.7f, visibleSize.height / 2));
	//enterButton->setTitleText("enter");
	enterButton->setTitleFontSize(7);
	enterButton->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::ENDED == type)
		{
			globalName = inputNameWindow->getString();
			auto transiton = TransitionSlideInL::create(2.f, RoomScene::create());
			Director::getInstance()->replaceScene(transiton);
		}
	});

	return true;
}

string NameScene::getName()
{
	assert(!globalName.empty());
	return globalName;
}
