#pragma once

#include "Unit.h"

class Unit;
class Building :public Unit {
	friend class UnitManager;

	bool isBuilding()override
	{
		return true;
	}
	
	virtual bool canCreateUnit()
	{
		return false;
	}

	virtual void localCreateUnit() = 0;
};