#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();

	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = HelloWorld::create();

	// 将HelloWorld层作为子节点添加到场景
	scene->addChild(layer);

	// 返回场景对象
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	// 获得设备支持的可见OpenGL视图大小（即窗口大小）。
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 关闭菜单
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// 删除触摸监听器
	auto removeAllTouchItem = MenuItemFont::create("TJ SSE 2018",
		[this](Ref* sender) {
		auto senderItem = static_cast<MenuItemFont*>(sender);
		senderItem->setString(" ");
		_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE);
	});
	removeAllTouchItem->setPosition(visibleSize.width / 2, visibleSize.height*0.2);
	// 创建菜单
	auto menu = Menu::create(removeAllTouchItem, closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	// 标题标签

	// 创建三个精灵对象
	auto sprite1 = Sprite::create("basic0_1_body.png");
	sprite1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(-80, 80));
	sprite1->setScale(0.1f);
	addChild(sprite1, 10);

	auto sprite2 = Sprite::create("MagentaSquare.png");
	sprite2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) +Vec2(-80, 80));
	sprite2->setVisible(false);
	addChild(sprite2, 20);

	/*auto sprite3 = Sprite::create("YellowSquare.png");
	sprite3->setPosition(Vec2(0, 0));
	sprite2->addChild(sprite3, 1);*/

	// ①创建一个单点触摸事件监听器，处理触摸事件逻辑
	auto listener1 = EventListenerTouchOneByOne::create();
	// 设置是否向下传递触摸
	listener1->setSwallowTouches(true);
	// 通过 lambda 表达式 直接实现触摸事件的响应函数
	listener1->onTouchBegan = [](Touch* touch, Event* event) {
		// 获得当前事件触摸的目标对象
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		// 获得当前的触摸点
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		// 获得触摸对象的ContentSize
		Size s = target->getContentSize();
		// 获得位置矩形
		Rect rect = Rect(0, 0, s.width, s.height);
		// 如果位置矩形包含触摸点
		if (rect.containsPoint(locationInNode))
		{
			log("onTouchBegan... x = %f, y = %f", locationInNode.x, locationInNode.y);
			// 设置透明度
			target->setOpacity(180);
			// 表明用户触摸事件已经被处理，后续的onTouchMoved、onTouchEnded和onTouchCancelled会接着响应，其他事件监听器则不会再去进行监听本次触摸事件。
			return true;
		}
		return false;
	};

	listener1->onTouchMoved = [](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		target->setVisible(true);
		// 移动触摸的精灵
		target->setPosition(target->getPosition() + touch->getDelta());
	};

	listener1->onTouchEnded = [this, sprite1](Touch* touch, Event* event) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto sprite3 = Sprite::create("basic0_1_body.png");
		sprite3->setPosition(target->getPosition());
		sprite3->setScale(0.3f);
		addChild(sprite3);
		target->setPosition(sprite1->getPosition());
		target->setVisible(false);
	};

	// 添加场景优先事件监听器。
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, sprite1);
	// ④注意：在使用addEventListenerWithSceneGraphPriority或者addEventListenerWithFixedPriority函数时，	会对当前使用的事件监听器添加一个已注册的标记，这使得它不能够被添加多次。所以当我们再次使用listener1的时候，需要使用clone()函数创建一个新的克隆。
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), sprite2);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), sprite3);

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
