#pragma once

#include "GridMap.h"
#include "cocos2d.h"
#include "UnitManager.h"

USING_NS_CC;

//矩形选框
struct MouseRect:public DrawNode {
	CREATE_FUNC(MouseRect);
	Point _touchStart, _touchEnd;

	Point start, end;//in the _tileMap
	//derive from DrawNode for memfun DrawRect
};

class MainScene :public Scene {
public:
	static MainScene* create();

	static MainScene* createScene();

	virtual bool init();

	//virtual void update(float delta);

private:
	int _playerId;

	MouseRect* _mouseRect;

	//单位管理器
	UnitManager* _unitManager;

	GridMap* _gridMap = nullptr;
	TMXTiledMap* _tiledMap = nullptr;
};

