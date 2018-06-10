#include "RealUnit.h"

Tank* Tank::create(const std::string& filename)
{
		Tank* ret = new Tank();

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
	_moveSpeed = 2;
	_attackCdMax = 60;
	_attackCd = _attackCdMax;

	_attackEffect = 50;
	_lifeValueMax = 200;
	_lifeValue = _lifeValueMax;

	initHp();

	_attackObject = "picture/ui/money.jpg";
	_state = Unit::WONDERING;
	schedule(schedule_selector(Unit::update));
}