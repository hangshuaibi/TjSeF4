#pragma once
//
//  CallFuncActionTest.h
//  ActionCallFuncTest
//
//  Created by Jason on 14-5-7.
//
//

#ifndef __ActionCallFuncTest__CallFuncActionScene__
#define __ActionCallFuncTest__CallFuncActionScene__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class CallFuncActionTest : public cocos2d::Layer
{
private:
	// 精灵
	Sprite * _plane;
	// 屏幕宽度、高度的变量
	int _screenWidth, _screenHeight;
public:
	/**
	* 静态函数，创建Scene场景
	* 注意：在Cocos2d-x中没有Cocos2d-iPhone中的“id”,建议返回类实例的指针
	*/
	static cocos2d::Scene* createScene();
	/**
	* init函数，完成初始化操作。
	* 注意：init函数在Cocos2d-x中返回bool值，而不是返回Cocos2d-iPhone中的“id”
	*/
	virtual bool init();

	// 自定义的move函数
	void move();

	/**
	* 使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/
	CREATE_FUNC(CallFuncActionTest);
};

#endif /* defined(__ActionCallFuncTest__CallFuncActionTest__) */
