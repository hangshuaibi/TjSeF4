#pragma once

#include "cocos2d.h"
#include "GridMap.h"
#include <map>

using std::map;
USING_NS_CC;

class Unit :public Sprite {
	enum State {
		WONDERING = 0,//发呆中
		MOVING,//移动
		ATTACKING,//攻击
		TRACING,//追击
	};

protected:
	//暂时只考虑移动
	int _state = 0;//上面的State
	//int traceId = 0;//看不爽的人Id
	float _moveSpeed;
	
	GridMap::GridVector _gridPath;//本单位当前路径

	void setProperities();

	void setGridPath(const GridMap::GridVector& gPath);

	//根据当前自己的状态发送消息给Manager
	//void sendInfoToManager();好像不用( •̀ ω •́ )y

	//根据state知道自己要干嘛
	void update(float delta);
	
	//运动的实际逻辑
	void move();
};

class UnitManager :public Ref {
private:
	map<int, Unit*> getUnitById;

	
};
