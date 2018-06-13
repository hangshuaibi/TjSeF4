#include"Building.h"
#include "PathFinder.h"
#include "NotGay.h"

void Building::addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap)
{
	_gridMap = gridMap;

	_tiledMap->addChild(this, 5);
}

Building* Building::create(const std::string& filename)
{
	Building *ret = new Building();

	if (ret && ret->initWithFile(filename))
	{
		ret->autorelease();

		return ret;
	}

	CC_SAFE_DELETE(ret);

	return nullptr;
}

