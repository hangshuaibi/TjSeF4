#pragma once

#include "GridMap.h"
#include "cocos2d.h"
#include "UnitManager.h"
#include "Data.h"

USING_NS_CC;

//¾ØÐÎÑ¡¿ò
struct MouseRect :public DrawNode {
	CREATE_FUNC(MouseRect);
	Point _touchStart, _touchEnd;

	Point start, end;//in the _tileMap
					 //derive from DrawNode for memfun DrawRect
};

class MainScene :public Scene {
public:

	friend class GameManager;
	static MainScene* create();

	static MainScene* createScene();

	virtual bool init();

	static int _screenWidth, _screenHeight;  //屏幕宽度和高度

	void update(float delta);

	//virtual void update(float delta);

private:

	cocos2d::Point _crusorPosition{ 0, 0 };
	int _playerId;

	MouseRect* _mouseRect;

	//µ¥Î»¹ÜÀíÆ÷
	UnitManager* _unitManager;

	GridMap* _gridMap = nullptr;

	TMXTiledMap* _tiledMap = nullptr;
};
