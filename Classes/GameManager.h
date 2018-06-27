#pragma once

/*这是游戏本地画面的管理者，主要功能是根据鼠标位置和AWSD键盘操作
进行屏幕的移动*/

#include <iostream>
#include <algorithm>
#include "cocos2d.h"
#include "MainScene.h"

USING_NS_CC;

class MainScene;
class GameManager :public cocos2d::Node
{
protected:
	MainScene* _mainScene;

	TMXTiledMap* _tiledMap = nullptr;

public:
	bool initWithScene(MainScene* mainScene);

	static GameManager* create(MainScene* mainScene);

	void focusOnBase();

	void scrollMap();

	std::string gameEncodeChat(std::string type, int id, std::string chatMessage);

	std::string gameDecodeChat(std::string message);
};