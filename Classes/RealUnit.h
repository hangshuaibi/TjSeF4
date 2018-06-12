#pragma once
#include "Unit.h"

USING_NS_CC;

class Unit;
class Tank :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Tank* create(int id);
		

	virtual void setProperties()override;
};

class Soldier :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Soldier* create(int id);


	virtual void setProperties()override;
};
