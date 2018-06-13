#pragma once
#include "cocos2d.h"
#include "GridMap.h"
#include <map>
#include "NotGay.h"
#include"string.h"
#include"ui/CocosGUI.h"

using namespace ui;

using std::map;
using std::string;
USING_NS_CC;

class UnitManager;
class Building :public Sprite {
	friend class UnitManager;
public:

	Building()
	{}

	void setTime(int time)
	{
		_time = time;
	}

	int getTime()
	{
		return _time;
	}

	void setGold(int gold)
	{
		_gold = gold;
	}

	int getGold()
	{
		return _gold;
	}

	void setElectricity(int electricity)
	{
		_electricity = electricity;
	}

	int getElectricity()
	{
		return _electricity;
	}

	void addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap);

	GridMap* _gridMap = nullptr;

	void setname(std::string name)
	{
		_buildingName = name;
	}

	std::string getname()
	{
		return _buildingName;
	}

	static Building* create(const std::string& filename);

	UnitManager* _unitManager = nullptr;

	LoadingBar* _bar;

	int _count=0;

	void update(float delta);
	

private:

	std::string _buildingName;
	
	int _time, _gold, _electricity;


};

