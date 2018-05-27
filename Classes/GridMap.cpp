#include "GridMap.h"

bool operator==(const Grid& lhs, const Grid& rhs)
{
	return lhs._x == rhs._x&&lhs._y == rhs._y;
}

Grid operator+(const Grid& lhs, const Grid&rhs)
{
	return Grid(lhs._x + rhs._x, lhs._y + rhs._y);
}

Grid operator-(const Grid& lhs, const Grid&rhs)
{
	return Grid(lhs._x - rhs._x, lhs._y - rhs._y);
}


Grid GridMap::findValidGridNear(const Grid& g)
{
	//先粗鄙一波，以后改
	return g;
}

Point GridMap::getPoint(const Grid& g)
{
	return Point(g._x * _gridWidth, g._y * _gridHeight);
}

Grid GridMap::getGrid(const Point& p)
{
	return Grid(p.x / _gridWidth, p.y / _gridHeight);
}

void GridMap::leaveGrid(int id, const Grid& g)
{
	assert(_occupiedId[g._x][g._y] == id);

	_occupiedId[g._x][g._y] = 0;
	_isOccupied[g._x][g._y] = 0;
}

void GridMap::occupyGrid(int id, const Grid& g)
{
	_isOccupied[g._x][g._y] = 1;
	_occupiedId[g._x][g._y] = id;
}

bool GridMap::tryToOccupyGrid(int id, const Grid& g)
{
	if (isGridOccupied(g))
	{
		return false;
	}

	occupyGrid(id, g);
	return true;
}

bool GridMap::isGridInMap(const Grid& g)
{
	return g._x >= 0 && g._x < _mapWidth&&g._y >= 0 && g._y < _mapHeight;
}



bool GridMap::isGridOccupied(const Grid& g)
{
	assert(isGridInMap(g)
		//, "Grid is out of map!"
	);

	return (_isOccupied[g._x][g._y]);
}

GridMap* GridMap::create(TMXTiledMap* tiledMap)
{
	GridMap* gridMap = new GridMap();
	if (gridMap&&gridMap->initWithTiledMap(tiledMap))
	{
		gridMap->autorelease();
		return gridMap;
	}

	CC_SAFE_DELETE(gridMap);
	log("error with init GridMap!\n");
	return nullptr;
}

bool GridMap::initWithTiledMap(TMXTiledMap* tiledMap)
{
	assert(tiledMap != 0);

	_mapWidth = tiledMap->getMapSize().width;
	_mapHeight = tiledMap->getMapSize().height;

	_gridWidth = tiledMap->getTileSize().width;
	_gridHeight = tiledMap->getTileSize().height;

	_pointOffset = Vec2(_gridWidth / 2, _gridHeight / 2);
	_isOccupied.assign(_mapWidth, vector<int>(_mapHeight, 0));
	_occupiedId.assign(_mapWidth, vector<int>(_mapHeight, 0));

	auto InfoLayer = tiledMap->getLayer("InfoLayer");
	assert(InfoLayer != 0);

	for (int gridX = 0;gridX < _mapWidth;++gridX)
	{
		for (int gridY = 0;gridY < _mapHeight;++gridY)
		{
			int gid = InfoLayer->getTileGIDAt(Vec2(gridX, _mapHeight - 1 - gridY));

			if (gid > 0)
			{
				_isOccupied[gridX][gridY] = 0;
			}
		}
	}

	return true;
}