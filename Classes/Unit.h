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
	//persudo friend class, for mainScene test
	friend class MainScene;
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


	int _id = 0;//每个Unit的id都是不一样的

	
	GridMap* _gridMap = nullptr;

	Grid _finalDest;//最终目的地
	Grid _curDest;//当前帧要去的点

	GridMap::GridVector _gridPath;//本单位当前路径

	void setProperties();

	void setGridPath(const GridMap::GridVector& gPath);//call by manager

	void setDestination(const Grid& g);//call by manager

	//maybe send message to network through manager in this function
	void findPath();

	void moveTest();

protected:
	void setManager(UnitManager* unitManager)
	{
		_unitManager = unitManager;
	}

	void setState(Unit::State state)
	{
		_state = state;
	}

	void autoAttack();

	//运动的实际逻辑
	void move();

	void shoot(/*string attackObject,*/Point end);

protected:
	UnitManager * _unitManager = nullptr;


	
	int _state = 0;//当前的状态
	float _moveSpeed;//移动速度
	int _attackCd;//距离上一次攻击的帧数
	int _attackCdMax;//每次攻击冷却需要的时间
	float _attackRange;//攻击范围

	/*-------------------未实现-----------------*/
	int _attackEffect;//攻击伤害
	int _lifeValue;//当前生命值
	int _lifeValueMax;//最大生命值


public:
	void update(float delta);
};

