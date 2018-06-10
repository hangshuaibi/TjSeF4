#include "Unit.h"
#include "PathFinder.h"
#include "NotGay.h"
#include "MessageTransfer/Encoder.h"

Unit::Unit():Sprite(),
_state(WONDERING),
_moveSpeed(0),
_gridMap(nullptr){}

void Unit::addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap)
{
	_gridMap = gridMap;

	_tiledMap->addChild(this, 5);
}

Unit* Unit::create(const std::string& filename)
{
	Unit *ret = new Unit();

	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

//弃用
void Unit::setProperties()
{
	_state = Unit::State::WONDERING;
	_attackRange = 200;
	_moveSpeed = 2;

	_attackCd = 3;
	_attackCdMax = 60;
}

void Unit::setGridPath(const GridMap::GridVector& path)
{
	_gridPath = path;
	assert(!path.empty());
	//先定一波终点
	_curDest = _gridPath.back();
}

void Unit::setDestination(const Grid& g)
{
	_finalDest = g;
}

void Unit::findPath()
{
	assert(_gridMap->isGridInMap(_finalDest));

	if (_finalDest == _gridMap->getGrid(getPosition()))
	{
		_gridPath = GridMap::GridVector();
		_curDest = Grid(-1, -1);

		return;
	}
	PathFinder pathFinder(_gridMap->_isOccupied, _gridMap->getGrid(getPosition()), _finalDest);
	

	_gridPath = pathFinder.getPath();
	_curDest = _gridPath.back();
}

GridMap::GridVector Unit::getPath(const Grid& dest)
{
	assert(_gridMap->isGridInMap(dest));
	if (dest == _gridMap->getGrid(getPosition()))
	{
		return GridMap::GridVector();
	}
	return PathFinder(_gridMap->_isOccupied,
		_gridMap->getGrid(getPosition()), dest).getPath();
}

void Unit::move()
{
	//起点在数组的尾部

	//路径为空时认为已经到达目的地
	if (_gridPath.empty())
	{
		setState(WONDERING);
		_curDest = Grid(-1, -1);//set flag for next time to initialize curDest

		return;
	}

	auto destPos = _gridMap->getPoint(_curDest);

	//方向
	auto dirVec = (destPos - getPosition()).getNormalized();
	//dirVec.normalize();//单位化

	//当前帧的偏移量
	auto curMoveVec = dirVec * _moveSpeed;

	auto nextPos = curMoveVec + getPosition();

	if (!_gridMap->isGridInMap(_curDest)
		||willBeyondDest(getPosition(),destPos,curMoveVec))
	{
		//在此不考虑越格，因为speed超过32太鬼畜了
		setPosition(nextPos);

		_curDest = _gridPath.back();
		_gridPath.pop_back();

		return;
	}
	
	auto nextGrid = _gridMap->getGrid(nextPos);
	auto curGrid = _gridMap->getGrid(getPosition());
	
/*---此处未实现占据格点的逻辑，因为当前寻路为静态寻路---*/
	if (nextGrid == curGrid)
	{
		setPosition(nextPos);
		return;
	}
	
	setPosition(nextPos);
}

void Unit::moveTest()
{
	if (_gridPath.empty())
		return;

	_curDest = _gridPath.back();
	_gridPath.pop_back();

	/*auto point = _gridMap->getPoint(_curDest);
	auto moveTo = MoveTo::create(5.0f, point + getParent()->getPosition());

	runAction(moveTo);*/
	setPosition(_gridMap->getPoint(_curDest));
}

void Unit::update(float delta)
{
	
	switch (_state) {
	case MOVING:
		//moveTest();
		move();
		break;
	case WONDERING:
		autoAttack();
		break;
	case TRACING:
		trace();
		break;
	default:
		break;
	}

	updateHp();//更新血条进度，血量更新的逻辑应该在manager做
}

//射击
void Unit::shoot(Unit* atkee)
{
	Point start = getPosition();
	Point end = atkee->getPosition();

	auto tiledMap = dynamic_cast<TMXTiledMap*>(getParent());

	assert(!_attackObject.empty());
	auto bullet = Sprite::create(_attackObject);

	assert(bullet != nullptr);

	assert(tiledMap != nullptr);

	bullet->setScale(0.01f);

	auto length = (end - start).length();
	auto duration = length / 32.0f;//子弹移动速度

	tiledMap->addChild(bullet, 5);
	bullet->setPosition(start);
	auto moveTo = MoveTo::create(duration, end);

	auto sequence = Sequence::create(moveTo, 
		CallFunc::create([=]() {
		bullet->setVisible(false);
		tiledMap->removeChild(bullet, true);
		atkee->_lifeValue -= _attackEffect;//承受伤害
	}), 
		NULL
	);
	
	bullet->runAction(sequence);
	
}

void Unit::autoAttack()
{
	//确认是否为友军
	if (!_unitManager->isOurBro(_id))
	{
		return;
	}
	assert(_id != 3);//>>>>>>>><<<<<<<<
	if (_attackCd == _attackCdMax)
	{
		for (auto item : _unitManager->_getUnitById)
		{

			if (_unitManager->isOurBro(item.first)||//友军
				!inAtkRange(item.second))
			{
				continue;
			}
			sendAttackMsg(item.first);

			//shoot(item.second->getPosition());
			break;
		}
	}

	if (++_attackCd > _attackCdMax)
		_attackCd = 0;
}

void Unit::initHp()
{
	_hpInterval = 100.0f / _lifeValueMax;
	_hp = LoadingBar::create("planeHP.png");
	assert(_hp != nullptr);
	assert(_unitManager != nullptr);

	_hp->setScale(0.06f, 0.1f);
	_hp->setDirection(LoadingBar::Direction::LEFT);
	_hp->setPercent(100);
	_unitManager->_tiledMap->addChild(_hp, 5);
	Point unitPos = getPosition();
	_hp->setPosition(Point(unitPos.x, 
		unitPos.y + _hp->getContentSize().height * 0.4f));
	
}

void Unit::updateHp()
{
	Point unitPos = getPosition();
	_hp->setPosition(Point(unitPos.x,
		unitPos.y + _hp->getContentSize().height * 0.4f));

	_hp->setPercent(_hpInterval*_lifeValue);
}

void Unit::sendAttackMsg(int targetId)
{
	//这个逻辑大概可以抽成一个函数，供autoatk和trace使用
	Encoder encoder("a", _id);
	std::string atkMsg = encoder.encodeAttack(targetId);
	_unitManager->_client->sendMessage(atkMsg);
}

void Unit::trace()
{
	//不是自己的unit则直接返回
	if (!_unitManager->isOurBro(_id))
	{
		return;
	}
	else if (_traceId == -1)
	{
		setState(WONDERING);
		return;
	}

	auto unit = _unitManager->_getUnitById.at(_traceId);
	if (unit == nullptr)
	{
		_traceId = -1;
		return;
	}


}

bool Unit::inAtkRange(Unit* unit)
{
	assert(unit!= nullptr);

	float distance =(this->getPosition() - unit->getPosition()).length();
	return distance <= _attackRange;
}