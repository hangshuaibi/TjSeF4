#include "Bullet.h"

Bullet* Bullet::create(Unit* atker, int targetId)
{
	auto ret = new Bullet();
	if (ret && ret->init(atker, targetId) && atker)
	{
		ret->autorelease();
		//ret->runBullet();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool Bullet::init(Unit* atker, int targetId)
{
	assert(atker != nullptr);
	_unitManager = atker->_unitManager;
	_picture = atker->_attackObject;
	_map = atker->getParent();
	assert(_map != nullptr);

	_effect = atker->_attackEffect;
	_start = atker->getPosition();

	if (_unitManager->_getUnitById.count(targetId) != 1)
	{
		return false;
	}
	_end = _unitManager->_getUnitById[targetId]->getPosition();
	_atkeeId = targetId;
	//speed

	auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(_picture);
	return initWithSpriteFrame(spriteFrame);
}

void Bullet::runBullet()
{
	auto time = (_end - _start).length() / _speed;
	auto moveTo = MoveTo::create(time, _end);

	_map->addChild(this, 5);
	setPosition(_start);
	auto sequence = Sequence::create(
		moveTo,
		CallFunc::create([=] {
		if (_unitManager->_getUnitById.count(_atkeeId) == 1)
			_unitManager->_getUnitById[_atkeeId]->_lifeValue -= _effect;
		this->setVisible(false);
	}),
		RemoveSelf::create(true),
		NULL
		);
	runAction(sequence);
}