#pragma once

#include "cocos2d.h"
#include "GridMap.h"
#include "UnitManager.h"
#include <map>

using std::map;
USING_NS_CC;


class UnitManager;
class Unit :public Sprite {
	
	friend class UnitManager;

public:
	//CREATE_FUNC(Unit);

	//default ctr
	Unit();//

	static Unit* create(const std::string& filename);
	
	enum State {
		WONDERING = 0,//发呆中，可能会触发自动攻击
		MOVING,//移动
		ATTACKING,//攻击
		TRACING,//追击
	};

//protected:
	void addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap);

	UnitManager* _unitManager = nullptr;

	int _id = 0;//每个Unit的id都是不一样的

	//暂时只考虑移动
	int _state = 0;//上面的State
	
	float _moveSpeed;
	
	GridMap* _gridMap = nullptr;

	Grid _finalDest;//最终目的地
	Grid _curDest;//当前帧要去的点

	GridMap::GridVector _gridPath;//本单位当前路径

	void setProperties();

	void setGridPath(const GridMap::GridVector& gPath);//call by manager

	void setDestination(const Grid& g);//call by manager

	//maybe send message to network through manager in this function
	void findPath();

	//运动的实际逻辑
	void move();

	void moveTest();


public:

	void setState(Unit::State state);

	//根据state知道自己要干嘛
	void update(float delta);
};


