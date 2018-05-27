#include "UnitManager.h"

bool UnitManager::init()
{
	return true;
}

bool UnitManager::hasSelectIdNow()
{
	return _selectId.empty();
}

void UnitManager::selectUnitByRect(const Rect& rect)
{
	assert(_gridMap != nullptr);

	if (hasSelectIdNow())
	{
		for (int id : _selectId)
		{
			auto pUnit = _getUnitById[id];
			auto point = Point(rect.getMidX(), rect.getMidY());
			pUnit->setDestination(_gridMap->getGrid(point));

			pUnit->findPath();
		}
	}

	for (const auto&idWithUnit : _getUnitById)
	{
		if (rect.containsPoint(idWithUnit.second->getPosition()))
		{
			_selectId.push_back(idWithUnit.first);
		}
	}
}