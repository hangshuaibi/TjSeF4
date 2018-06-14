#pragma once
#include "Unit.h"
#include "Building.h"

USING_NS_CC;

class Building;
class Unit;
class Tank :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Tank* create();
		

	virtual void setProperties()override;
};

class Soldier :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Soldier* create();


	virtual void setProperties()override;
};

class SoldierCamp :public Building {
	friend class UnitManager;

public:
	static SoldierCamp* create();

private:
	virtual void setProperties()override;

	virtual void localCreateUnit()override;

	virtual bool canCreateUnit()override
	{
		return true;
	}
};