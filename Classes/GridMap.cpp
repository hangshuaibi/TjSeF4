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

GridMap::GridVector GridMap::findValidGridsNear(const Grid& g, int size)
{
	//若只找一个点，逻辑交给findValidGridNear去做
	if (size == 1)
	{
		return GridVector{ findValidGridNear(g) };
	}

	GridVector gridVector;
	if (isGridValid(g))
	{
		gridVector.push_back(g);
		--size;
	}

	//菱形查找
	GridVector dirVec = { Grid(1,1),Grid(1,-1),Grid(-1,-1),Grid(-1,1) };

	for (int i = 0;i < _mapWidth + _mapHeight;++i)
	{
		Grid nearg = g + Grid(-i, 0);
		for (int dir = 0;dir < 4;++dir)
		{
			for (int j = 0;j < i;++j)
			{
				nearg = nearg + dirVec[dir];
				if (isGridValid(nearg))
				{
					gridVector.push_back(nearg);
					if (--size == 0)
					{
						return gridVector;
					}
				}
				//one grid one time 
			}
		}
		//main loop
	}
	//control won't arrive here in normal state, return a empty vector
	return GridVector();
}


Grid GridMap::findValidGridNear(const Grid& g)
{
	if (isGridValid(g))
	{
		return g;
	}

	//菱形查找
	GridVector dirVec = { Grid(1,1),Grid(1,-1),Grid(-1,-1),Grid(-1,1) };

	for (int i = 0;i < _mapWidth + _mapHeight;++i)
	{
		Grid nearg = g + Grid(-i, 0);
		for (int dir = 0;dir < 4;++dir)
		{
			for (int j = 0;j < i;++j)
			{
				nearg = nearg + dirVec[dir];
				if (isGridValid(nearg))
				{
					return nearg;
				}
			}
		}
	}

	return Grid(-1, -1);
}

Point GridMap::getPoint(const Grid& g)
{
	return Point(g._x * _gridWidth, g._y * _gridHeight) 
		+ _pointOffset;//偏移量
}

Grid GridMap::getGrid(const Point& p)
{
	return Grid((int)p.x / _gridWidth, (int)p.y / _gridHeight);
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

	//修正格点宽度和长度
	_gridWidth = tiledMap->getTileSize().width / CC_CONTENT_SCALE_FACTOR();
	_gridHeight = tiledMap->getTileSize().height / CC_CONTENT_SCALE_FACTOR();

	log("gridwidth %f, gridheight %f, offset %f", _gridWidth, _gridHeight, CC_CONTENT_SCALE_FACTOR());

	_pointOffset = Vec2(_gridWidth / 2.0f, _gridHeight / 2.0f);
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
				//
				//CC_ASSERT(0);
				_isOccupied[gridX][gridY] = 1;

				//log("(%d, %d)   ", gridX, _mapHeight - 1 - gridY);
			}
		}
	}

	return true;
}

bool GridMap::isGridValid(const Grid& g)
{
	return isGridInMap(g) && (_isOccupied[g._x][g._y] == 0);
}

bool GridMap::isAreaValid(const GridVector& area)
{
	for (auto& grid : area)
	{
		if (!isGridValid(grid))//有一个不合法则该区域不合法
		{
			return false;
		}
	}
	return true;
}

GridMap::GridVector GridMap::getArea(const Size& size, const Point& pos)
{
	Point start_(pos.x - size.width / 2, pos.y - size.height / 2),
		end_(pos.x + size.width / 2, pos.y + size.height / 2);

	GridMap::GridVector area;
	Grid start = getGrid(start_), end = getGrid(end_);
	for (int x = start._x;x <= end._x;++x)
	{
		for (int y = start._y;y <= end._y;++y)
		{
			area.push_back(Grid(x, y));
		}
	}

	return area;
}

void GridMap::setAreaOccupied(const GridVector& area)
{
	for (const auto& grid : area)
	{
		assert(isGridValid(grid));
		setGridOccupied(grid);
	}
}

void GridMap::setAreaValid(const GridVector& area)
{
	for (const auto& grid : area)
	{
		assert(!isGridValid(grid));
		setGridValid(grid);
	}
}

void GridMap::setGridOccupied(const Grid& grid)
{
	_isOccupied[grid._x][grid._y] = 1;
}

void GridMap::setGridValid(const Grid& grid)
{
	_isOccupied[grid._x][grid._y] = 0;
}