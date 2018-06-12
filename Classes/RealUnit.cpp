#include "RealUnit.h"

Tank* Tank::create(int id)
{
		Tank* ret = new Tank();

		static const std::string filename = "tank.png";

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

	static const std::string filename = "soldier.png";

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
	_attackRange = 64;
	_moveSpeed = 0.8f;
	_attackCdMax = 40;
	_attackCd = _attackCdMax;

	_attackEffect = 30;
	_lifeValueMax = 100;
	_lifeValue = _lifeValueMax;

	initHp();

	_attackObject = "picture/ui/gold.png";
	_state = Unit::WONDERING;
	schedule(schedule_selector(Unit::update));
}