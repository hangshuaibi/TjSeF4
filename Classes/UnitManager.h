#pragma once

#include "cocos2d.h"
#include <map>
#include "Unit.h"
#include "Data.h"//for MAX_PLAYER_NUM
#include "NotGay.h"

using std::map;
USING_NS_CC;

class Unit;
class UnitManager:public Node {
	//test'
	friend class MainScene;

private:
	Point _basePos = Point(100, 100);

	int _playerId = 0;

	//add a MAX_PLAYER_NUM every time, initialize with playerId
	int _nextId = -1;
public:
	bool init();
	CREATE_FUNC(UnitManager);
	void selectUnitByRect(const Rect& rect);

	map<int, Unit* > _getUnitById;
	//天选之子
	vector<int> _selectId;
	bool hasSelectIdNow();

	GridMap* _gridMap = nullptr;
	TMXTiledMap* _tiledMap = nullptr;

	//出生点默认在基地
	//这个是本地版本的create，产生unit的id并将信息传递给client
	void createUnit(int type /*const Grid& g*/);

	int getNextId();

	//清空选择
	void selectId(const Rect& rect);

	//做矩形框选的实际工作
	void abandonSelectedId();

	//检测id对应的Unit是不是自己人
	bool isOurBro(int id);

	/*~UnitManager()
	{
		_gridMap = nullptr;
	}*/
};