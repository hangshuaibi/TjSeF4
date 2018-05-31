#pragma once

#include "cocos2d.h"
#include <map>
#include "Unit.h"

using std::map;
USING_NS_CC;

class Unit;
class UnitManager:public Node {
public:
	int _playerId = 0;

	bool init();
	CREATE_FUNC(UnitManager);
	void selectUnitByRect(const Rect& rect);

	map<int, Unit* > _getUnitById;
	//天选之子
	vector<int> _selectId;
	bool hasSelectIdNow();

	GridMap* _gridMap = nullptr;

	~UnitManager()
	{
		_gridMap = nullptr;
	}
};