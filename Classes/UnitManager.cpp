#include "UnitManager.h"
#include <utility>//for make_pair
//#include "GridMap.h"

bool UnitManager::init()
{
	//set the playerId
	_playerId = 0;//联网后此处得改

	_nextId = _playerId;

	return true;
}

bool UnitManager::hasSelectIdNow()
{
	return !_selectId.empty();
}

void UnitManager::selectUnitByRect(const Rect& rect)
{
	assert(_gridMap != nullptr);

	if (hasSelectIdNow())
	{
		if (rectTooSmall(rect))
		{	
			assert(_gridMap != nullptr);

			//dest grid
			auto destPoint = Point(rect.getMidX(), rect.getMidY());
			auto destGrid = _gridMap->getGrid(destPoint);

			//find valid grid around dest depend on size
			int size = _selectId.size();
			GridMap::GridVector gridVector = _gridMap->findValidGridsNear(destGrid, size);

			//确认返回了足够大的数组
			assert(gridVector.size() == size);

			for (int id : _selectId)
			{
				auto pUnit = _getUnitById[id];
				if (pUnit == nullptr)
				{
					//maybe this unit is dead
					continue;
				}
				
				pUnit->setState(Unit::WONDERING);
				pUnit->setDestination(gridVector[--size]);

				pUnit->findPath();
				pUnit->schedule(schedule_selector(Unit::update));

				//send message to client
				
				//remove it when network is supported
				pUnit->setState(Unit::MOVING);
			}
		}
		else {//重新框选
			abandonSelectedId();
			selectId(rect);
		}
		return;
	}

	//无选中单位时首次框选
	selectId(rect);
}

void UnitManager::selectId(const Rect& rect)
{
	for (const auto&idWithUnit : _getUnitById)
	{
		if (isOurBro(idWithUnit.first) && //自己人才享有控制权
			rect.containsPoint(idWithUnit.second->getPosition()))
		{
			_selectId.push_back(idWithUnit.first);

			log("%d", (int)idWithUnit.second->getOpacity());
			//设置透明度表示选中
			idWithUnit.second->setOpacity(100);
		}
	}
}

void UnitManager::localCreateUnit(int/* type*/)
{

	assert(_gridMap != nullptr);
	assert(_tiledMap != nullptr);

	auto unit = Unit::create("fighter.png");
	unit->addToMap(_gridMap, _tiledMap);

	auto baseGrid = _gridMap->getGrid(_basePos);
	auto nearValidBaseGrid = _gridMap->findValidGridNear(baseGrid);

	//unit has been tiledMap's child
	unit->setPosition(_gridMap->getPoint(nearValidBaseGrid));
	unit->setManager(this);
	
	int id = getNextId();
	_getUnitById.insert(std::make_pair(id, unit));

	unit->setProperties();
}

int UnitManager::getNextId()
{
	int retId = _nextId;

	_nextId += MAX_PLAYER_NUM;

	return retId;
}

void UnitManager::abandonSelectedId()
{
	for (int id : _selectId)
	{
		//原始透明度是255
		_getUnitById[id]->setOpacity(255);
	}

	_selectId.clear();
}

bool UnitManager::isOurBro(int id)
{
	return id%MAX_PLAYER_NUM == _playerId;
}