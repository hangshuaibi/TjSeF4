//#include "CallFuncActionTest.h"
//
//  CallFuncActionTest.cpp
//  ActionCallFuncTest
//
//  Created by Jason on 14-5-7.
//
//

#include "CallFuncActionTest.h"

USING_NS_CC;

// 静态变量，表示plane的tag
static int kTagPlane = 1;

Scene* CallFuncActionTest::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();

	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = CallFuncActionTest::create();

	// 将CallFuncActionScene层作为子节点添加到场景
	scene->addChild(layer);

	// 返回场景对象
	return scene;
}

bool CallFuncActionTest::init()
{
	// 调用父类的init函数
	if (!Layer::init())
	{
		return false;
	}
	// 获得OpenGL ES视图的大小
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 将屏幕宽度、高度保存到定义的变量当中
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;
	// 使用.png图片创建一个精灵
	_plane = Sprite::create("plane.png");
	// 设置精灵的位置
	_plane->setPosition(Vec2(_plane->getContentSize().width / 2, _screenHeight / 2));
	// 将精灵添加为当前层的子节点
	this->addChild(_plane, 1, kTagPlane);
	/**
	定义了一个CallFunc动作，调用move函数
	3.0 后新的回调接口，由四个CC_CALLBACK取代。
	其实CC_CALLBACK的差别就在于后面的数字
	0就代表回调的是没有参数的函数，1就是有一个参数，2就是有两个参数，3就是有三个参数
	*/
	// auto callFunc = CallFunc::create(CC_CALLBACK_0(CallFuncActionScene::move, this));
	// C++11的lambda表达式实现函数回调动作
	auto callFunc = CallFunc::create([&] {
		// 定义一个MoveTo动作，精灵从屏幕左边移动到屏幕右边
		auto moveTo = MoveTo::create(3, Vec2(_screenWidth - _plane->getContentSize().width / 2, _screenHeight / 2));
		// 精灵（飞机）执行MoveTo动作
		_plane->runAction(moveTo);
	});
	// 精灵（飞机）执行CallFunc回调动作
	_plane->runAction(callFunc);
	return true;
}

// 实现自定义的move函数
void CallFuncActionTest::move() {
	// 通过tag值获得精灵对象
	auto plane = this->getChildByTag(kTagPlane);
	// 定义一个MoveTo动作，精灵从屏幕左边移动到屏幕右边
	auto moveTo = MoveTo::create(3, Vec2(_screenWidth - plane->getContentSize().width / 2, _screenHeight / 2));
	// 精灵（飞机）执行MoveTo动作
	plane->runAction(moveTo);
}
