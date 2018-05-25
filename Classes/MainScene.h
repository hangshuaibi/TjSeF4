#pragma once

#include "GridMap.h"
#include "cocos2d.h"

USING_NS_CC;

class MainScene :public Scene {
public:
	static MainScene* create();

	static MainScene* createScene();

	virtual bool init();

	//virtual void update(float delta);

private:
	int _playerId;

	GridMap* _gridMap = nullptr;
	TMXTiledMap* _tiledMap = nullptr;
};

