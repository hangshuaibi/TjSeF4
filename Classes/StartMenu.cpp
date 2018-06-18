
#include "StartMenu.h"
#include "ui/CocosGUI.h"
#include "ChooseScene.h"

USING_NS_CC;
using namespace ui;

Scene* StartMenu::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = StartMenu::create();
	// 将StartMenu层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool StartMenu::init() {
	// 调用父类的init方法
	if (!Layer::init()) {
		return false;
	}
	// 获得设备可见视图大小
	Size visibleSize = Director::getInstance()->getVisibleSize();


	auto bg = Sprite::create("bg.png");
	bg->setScaleY(2);
	bg->setScaleX(3);
	this->addChild(bg);
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	// “开始游戏”按钮
	auto startButton = Button::create("button.png");
	startButton->setScale(2.2);
	startButton->setTitleText("Start");
	startButton->setTitleFontName("微软雅黑");
	startButton->setTitleFontSize(12);
	startButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.8));
	startButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			// 切换到PreLoad场景
			auto transition = TransitionSlideInL::create(2.0, ChooseScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(startButton);

	// “游戏设置”按钮
	auto setButton = Button::create("button.png");
	setButton->setScale(2.2);
	setButton->setTitleText("Settings");
	setButton->setTitleFontName("微软雅黑");
	setButton->setTitleFontSize(12);
	setButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.55));
	setButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			// 切换到AudioControl场景
			
		}
	});
	this->addChild(setButton);

	// “退出游戏”按钮
	auto closeButton = Button::create("button.png");
	closeButton->setScale(2.2);
	closeButton->setTitleText("Quit");
	closeButton->setTitleFontName("微软雅黑");
	closeButton->setTitleFontSize(12);
	closeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.3));
	closeButton->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
			return;
#endif
			Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
#endif

		}
	});
	this->addChild(closeButton);

	return true;
}












