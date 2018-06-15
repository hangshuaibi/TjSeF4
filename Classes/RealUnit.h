#pragma once
#include "Unit.h"
#include "Building.h"
#include "MainScene.h"

USING_NS_CC;

class ControlPanel;
class Building;
class Unit;
class Tank :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Tank* create(int id);
		
	virtual void setProperties()override;
};

class Fighter :public Unit {
	friend class UnitManager;
public:
	static Fighter* create(int id);

	virtual void setProperties()override;

	GridMap::GridVector getPath(const Grid& dest)override
	{
		auto start = _gridMap->getGrid(getPosition());
		GridMap::GridVector path = { dest };

		return path;
	}
};

class Soldier :public Unit {
	friend class UnitManager;
	//persudo friend class, for mainScene test
	friend class MainScene;
public:
	static Soldier* create(int id);


	virtual void setProperties()override;
};

class SoldierCamp :public Building {
	friend class UnitManager;

	int _maxCreateNum;//同一时间内最大生产数量
	int _curCreateNum;
public:
	static SoldierCamp* create(int id);

private:
	virtual void setProperties()override
	{
		_lifeValueMax = 300;
		_lifeValue = _lifeValueMax;
		_maxCreateNum = 3;
		_curCreateNum = 0;

		Building::setProperties();

		initHp();
		schedule(schedule_selector(Unit::update));
	}

	virtual void localCreateUnit()override
	{
		if (_curCreateNum >= _maxCreateNum)
		{
			return;
		}
		
		++_curCreateNum;
		
		auto validPos = findCreatePos();

		_unitManager->localCreateUnit(Type::SOILDER, validPos);
		auto sequence = Sequence::create(
			DelayTime::create(2.0f),
			CallFunc::create([=] {
			--_curCreateNum;
		}),
			NULL
			);
		runAction(	sequence);
	}

	virtual bool canCreateUnit()override
	{
		return true;
	}
};

class Base :public Building {
public:
	static Base* create(int id);
	void setProperties()override
	{
		Building::setProperties();
		initHp();

		_lifeValueMax = 1000;
		_lifeValue = _lifeValueMax;
		schedule(schedule_selector(Unit::update));
	}
	void localCreateUnit()override
	{/*empty*/ }
};

class Factory :public Building {
	friend class UnitManager;
	friend class ControlPanel;

	int _maxCreateNum;//同一时间内最大生产数量
	int _curCreateNum;
public:
	static Factory* create(int id);

private:
	virtual void setProperties()override
	{
		_lifeValueMax = 500;
		_lifeValue = _lifeValueMax;

		_maxCreateNum = 2;
		_curCreateNum = 0;

		Building::setProperties();

		initHp();
		schedule(schedule_selector(Unit::update));
	}

	virtual void localCreateUnit()override
	{ 
		//所有factory共用一个panel
		static ControlPanel* panel = _unitManager->_controlPanel;

		auto panelPos = getPosition()
			//+ _unitManager->_tiledMap->getPosition()
			- panel->getContentSize() / 2;

		if (!panel->isVisible())
		{
			panel->setPosition(panelPos);
			panel->setVisible(true);
			panel->_factory = this;
		}
		else {
			if (panel->getPosition() == panelPos)
			{
				panel->setVisible(false);
			}
			else {
				panel->setPosition(panelPos);
				panel->_factory = this;
			}
		}
	}

	virtual bool canCreateUnit()override
	{
		return true;
	}

	void localCreateUnit_(Type type)
	{
		if (_curCreateNum >= _maxCreateNum)
		{
			return;
		}
		++_curCreateNum;
		auto validpos = findCreatePos();
		_unitManager->localCreateUnit(type, validpos);
		auto sequence = Sequence::create(
			DelayTime::create(2.0f),
			CallFunc::create([=] {
			--_curCreateNum;
		}),
			NULL
			);
		runAction(sequence);
	}
};