#pragma once

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

};
