#pragma once

#include "Unit.h"

class Unit;
class Building :public Unit {
	friend class UnitManager;
	Type _createType = Type::FIGHTER;
protected:
	GridMap::GridVector _occupiedArea;//建筑占据区域

protected:
	void setOccupiedArea(GridMap::GridVector area)override
	{
		_occupiedArea = area;
	}
	GridMap::GridVector getOccupiedArea()override
	{
		return _occupiedArea;
	}

	void setProperties()override
	{
		_state = Unit::State::NOSTATE;
		_moveSpeed = 0.f;
		_attackEffect = 0;

		initHp();
		schedule(schedule_selector(Unit::update));
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

		//------------------//
		auto pos = getPosition();
		assert(pos != Point(0.f, 0.f));//不是默认位置
		auto area = _gridMap->getArea(getContentSize()*0.4f, pos);
		_gridMap->setAreaOccupied(area);//加入地图时把格点区域覆盖为1
		setOccupiedArea(area);//设置单位本身的占据区域
		//-----------------//

		/*auto grid = _gridMap->getGrid(getPosition());
		assert(!_gridMap->isGridOccupied(grid));
		_gridMap->_isOccupied[grid._x][grid._y] = 1;//标识建筑物所在格点*/
	}

	Point findCreatePos()
	{
		auto grid = _gridMap->getGrid(getPosition());
		auto validGrid = _gridMap->findValidGridNear(grid);
		auto validPos = _gridMap->getPoint(validGrid);

		return validPos;
	}
};