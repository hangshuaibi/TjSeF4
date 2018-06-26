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

		std::string filename = "tank";
		filename.append(suffix[id%MAX_PLAYER_NUM]);
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

		if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

	_attackObject = "bullet_t.png";
	_state = Unit::WONDERING;
	schedule(schedule_selector(Unit::update));
}

Soldier* Soldier::create(int id)
{
	Soldier* ret = new Soldier();

	std::string filename = "soldier";
	filename.append(suffix[id%MAX_PLAYER_NUM]);
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

	_attackObject = "bullet_s.png";
	_state = Unit::WONDERING;
	schedule(schedule_selector(Unit::update));
}

SoldierCamp* SoldierCamp::create(int id)
{
	SoldierCamp* ret = new SoldierCamp();

	std::string filename = "soldiercamp";
	filename.append(suffix[id%MAX_PLAYER_NUM]);
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

	std::string filename = "basement.png";
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

	std::string filename = "fighter";
	filename.append(suffix[id%MAX_PLAYER_NUM]);
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

		_attackObject = "bullet_f.png";
		_state = Unit::WONDERING;
		schedule(schedule_selector(Unit::update));
}

Factory* Factory::create(int id)
{
	Factory* ret = new Factory();

	std::string filename = "factory";
	filename.append(suffix[id%MAX_PLAYER_NUM]);
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

	std::string filename = "barracks.png";
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
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

	std::string filename = "storage.png";
	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);

	if (ret&&ret->initWithSpriteFrame(spriteFrame))
	{
		ret->autorelease();

		return ret;
	}
	assert(0);
	CC_SAFE_DELETE(ret);
	return nullptr;
}