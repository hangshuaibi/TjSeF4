#pragma once
#include "Unit.h"

USING_NS_CC;

class Unit;
class Tank :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Tank* create(const std::string& filename);
		

	virtual void setProperties()override;
};
