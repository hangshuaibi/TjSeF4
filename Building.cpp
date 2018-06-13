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


void Building::update(float delta)
{
	_count++;
	//ÐÞ¸Ä½ø¶È
	_bar->setPercent(100/ _time * _count);
	if (_bar->getPercent() >= 100)
	{
		_bar->setVisible(0);
		this->removeChild(_bar,5);
		_count = 0;
	}
}

