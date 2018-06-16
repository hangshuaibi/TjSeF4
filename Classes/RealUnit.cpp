#include "RealUnit.h"
#include <string>
#include "Data.h"


static std::string unitName[10] = {
	"tank","soldier","fighter","soldier","factory","base","tower",
};
static std::string suffix[MAX_PLAYER_NUM] = {
	"_0.png","_1.png","_2.png","_3.png",
};

Tank* Tank::create(int id)
{
		Tank* ret = new Tank();

		std::string filename = "picture/units/tank";
		filename.append(suffix[id%MAX_PLAYER_NUM]);

		if (ret&&ret->initWithFile(filename))
		{
			ret->autorelease();

			return ret;
		}

		CC_SAFE_DELETE(ret);
		return nullptr;
}

void Tank::setProperties()
{
	_attackRange = 64;
	_moveSpeed = 0.3f;
	_attackCdMax = 60;
	_attackCd = _attackCdMax;

	_attackEffect = 50;
	_lifeValueMax = 500;
	_lifeValue = _lifeValueMax;

	initHp();

	_attackObject = "picture/ui/gold.png";
	_state = Unit::WONDERING;
	schedule(schedule_selector(Unit::update));
}

Soldier* Soldier::create(int id)
{
	Soldier* ret = new Soldier();

	std::string filename = "picture/units/soldier";
	filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Soldier::setProperties()
{
	_attackRange = 32;
	_moveSpeed = 0.8f;
	_attackCdMax = 40;
	_attackCd = _attackCdMax;

	_attackEffect = 30;
	_lifeValueMax = 200;
	_lifeValue = _lifeValueMax;

	initHp();

	_attackObject = "picture/ui/gold.png";
	_state = Unit::WONDERING;
	schedule(schedule_selector(Unit::update));
}

SoldierCamp* SoldierCamp::create(int id)
{
	SoldierCamp* ret = new SoldierCamp();

	std::string filename = "picture/units/soldiercamp";
	filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	assert(0);
	CC_SAFE_DELETE(ret);
	return nullptr;
}

Base* Base::create(int id)
{
	Base* ret = new Base();

	std::string filename = "picture/units/basement.png";
	//filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	assert(0);//控制不会到达这里
	CC_SAFE_DELETE(ret);
	return nullptr;
}

Fighter* Fighter::create(int id)
{
	Fighter* ret = new Fighter();

	std::string filename = "picture/units/fighter";
	filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Fighter::setProperties()
{
		_attackRange = 96;
		_moveSpeed = 0.6f;
		_attackCdMax = 80;
		_attackCd = _attackCdMax;

		_attackEffect = 100;
		_lifeValueMax = 400;
		_lifeValue = _lifeValueMax;

		initHp();

		_attackObject = "picture/ui/gold.png";
		_state = Unit::WONDERING;
		schedule(schedule_selector(Unit::update));
}

Factory* Factory::create(int id)
{
	Factory* ret = new Factory();

	std::string filename = "picture/units/factory";
	filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	assert(0);
	CC_SAFE_DELETE(ret);
	return nullptr;
}

Mine* Mine::create(int id)
{
	Mine* ret = new Mine();

	std::string filename = "picture/units/barracks.png";
	//filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	assert(0);
	CC_SAFE_DELETE(ret);
	return nullptr;
}

ElectricityFactory* ElectricityFactory::create(int id)
{
	ElectricityFactory* ret = new ElectricityFactory();

	std::string filename = "picture/units/storage.png";
	//filename.append(suffix[id%MAX_PLAYER_NUM]);

	if (ret&&ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}
	assert(0);
	CC_SAFE_DELETE(ret);
	return nullptr;
}