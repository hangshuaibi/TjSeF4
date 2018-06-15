#pragma once

#include "Unit.h"

class Unit;
class Building :public Unit {
	friend class UnitManager;
	Type _createType = Type::FIGHTER;
protected:
	void setProperties()override
	{
		_state = Unit::State::NOSTATE;
		_moveSpeed = 0.f;
		_attackEffect = 0;
	}

	bool isBuilding()override
	{
		return true;
	}
	
	virtual bool canCreateUnit()
	{
		return false;
	}

	virtual void localCreateUnit() = 0;
	void addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap)override
	{
		Unit::addToMap(gridMap, _tiledMap);//调用父类的addToMap方法

		auto grid = _gridMap->getGrid(getPosition());
		//assert(_gridMap->isGridInMap(grid));
		assert(!_gridMap->isGridOccupied(grid));
		_gridMap->_isOccupied[grid._x][grid._y] = 1;//标识建筑物所在格点
	}

	Point findCreatePos()
	{
		auto grid = _gridMap->getGrid(getPosition());
		auto validGrid = _gridMap->findValidGridNear(grid);
		auto validPos = _gridMap->getPoint(validGrid);

		return validPos;
	}
};