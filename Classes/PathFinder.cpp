#include "PathFinder.h"
#include "NotGay.h"


PathFinder::PathFinder(const vector<vector<int> >&gridTable,
	const Grid& start, const Grid& end) :
	_mapWidth(gridTable.size()),_mapHeight(gridTable[0].size()),
	 _start(nullptr),
	_end(nullptr) {
	//初始化格点表
	_gridTable.assign(_mapWidth,
		vector<grid>(_mapHeight, grid()));
	//初始化起点终点指针
	_start = &_gridTable[start._x][start._y];
	_end = &_gridTable[end._x][end._y];
	//循环计算g值需要用到终点坐标，循环内会再计算一次终点的xy，无伤大雅
	//_start->x = start._x, _start->y = start._y;
	_end->x = end._x, _end->y = end._y;

	for (int i = 0; i < _mapWidth;++i)
	{
		for (int j = 0;j < _mapHeight;++j)
		{
			grid& rg = _gridTable[i][j];
			rg.x = i, rg.y = j;		

			rg.hValue = calculateHValue(&rg, _end);

			if (gridTable[i][j] == 1)//default state is UNOCCUPIED
			{
				//log("(%d, %d)   ", i, j);

				setState(&rg, grid::OCCUPIED);
			}
		}
	
	}

	assert(_end != nullptr&&_start != nullptr);

	setState(_start, grid::State::START);
	setState(_end, grid::State::END);

	_start->gValue = 0;

	_end->hValue = 0;//already calculated in the for loop

	log("start(%d, %d)\nend(%d, %d)", start._x, start._y, end._x, end._y);

	log("per grid width %f", CC_CONTENT_SCALE_FACTOR());
	//Initialize openlist
	_openList.push_back(_start);
}

bool PathFinder::isGridValid(const grid* pg)
{
	int x = pg->x, y = pg->y;

	if (x < 0 || x >= _mapWidth ||
		y < 0 || y >= _mapHeight)
	{
		return false;
	}

	grid::State state = _gridTable[x][y].state;
	return state != grid::OCCUPIED&&state != grid::INCLOSELIST;
}

void PathFinder::checkSurroundGrid(const grid* pg)
{
	for (int i = 0;i < DIRECTION_NUM;++i)
	{
		int x =pg->x + DIRECTIONS[i][0], y = pg->y + DIRECTIONS[i][1];

		if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
		{
			continue;//出界
		}
		//pointer to candidate grid
		grid* pcg = &_gridTable[x][y];
		if (!isGridValid(pcg))//
			//|| !checkCorner(pg, pcg))
		{
			//log("(%d %d)", x, y);/////////////////////////
			continue;
		}
		//remember hValue of every grid has been calculated in PathFindther constructor
		int gValueByPg = calculateGValue(pg, pcg);
		if (gValueByPg + pg->gValue< pcg->gValue)
		{
			pcg->parent = const_cast<grid*>(pg);
			pcg->gValue = gValueByPg + pg->gValue;
			pcg->fValue = pcg->gValue + pcg->hValue;
		}
	
		if (!isInOpenList(pcg))
		{
			_openList.push_back(pcg);
			//flag
			setState(pcg, grid::INOPENLIST);
		}

		assert(pcg->parent != nullptr);
	}
}

bool PathFinder::checkCorner(const grid* lhs, const grid* rhs)
{
	assert(isGridValid(lhs) && isGridValid(rhs));

	if (lhs->x == rhs->x || lhs->y == rhs->y)
	{
		return true;
	}

	if (_gridTable[lhs->x][rhs->y].state == grid::OCCUPIED ||
		_gridTable[rhs->x][lhs->y].state == grid::OCCUPIED)
	{
		return false;
	}

	return true;
}

void PathFinder::setState(grid* g, grid::State state)
{
	g->state = state;
}

bool PathFinder::isInCloseList(const grid* g)
{
	return g->state == grid::INCLOSELIST;
}

bool PathFinder::isInOpenList(const grid*g)
{
	return g->state == grid::INOPENLIST;
}

void PathFinder::removeFromOpenList(grid* g)
{
	assert(g != nullptr);
	for (auto it = _openList.begin();it!=_openList.end();++it)
	{
		if (g != *it)
		{
			continue;
		}

		_openList.erase(it);
		setState(g, grid::INCLOSELIST);

		return;
	}
}

int PathFinder::calculateGValue(const grid* parent, const grid* cur)
{
	if (parent->x == cur->x ||
		parent->y == cur->y)
	{
		return DISTANCE0;//10
	}
	return DISTANCE1;//14
}

int PathFinder::calculateHValue(const grid*cur, const grid* end)
{
	return (abs(cur->x - end->x) + abs(cur->y - end->y))*DISTANCE0;//10
}

PathFinder::grid* PathFinder::nextToSearch()
{
	assert(!_openList.empty());

	grid* ret = _openList.front();

	for (auto pg : _openList)
	{
		if (pg->fValue < ret->fValue)
		{
			ret = pg;
		}
	}
	return ret;
}

vector<Grid> PathFinder::getPath()
{
	if (!searchPath())
	{
		assert("no path");	
	}
	generatePath();

	return _resultPath;
}

void PathFinder::generatePath()
{
	grid* cur = _end;
	
	/*这里传出去的路径起点（的前一个grid）在尾部，终点在头部*/

	//在此起点并不会被push_back
	while (cur->parent != nullptr)//cur is not the FATHER
	{
		_resultPath.push_back(Grid(cur->x, cur->y));

		//log("my path: %d %d \n", cur->x, cur->y);

		cur = cur->parent;
	}

	
}

bool PathFinder::searchPath()
{
	while (!_openList.empty())
	{
		grid* next = nextToSearch();
		if (next == _end)return true;
		removeFromOpenList(next);
		checkSurroundGrid(next);
	}

	return false;
}

void PathFinder::optimizePath()
{
	if (_resultPath.size() < 3)
	{
		return;
	}

	Grid prevDiretion(0, 0);
	Grid prevGrid = _resultPath.back();
	for (auto it = _resultPath.end() - 2;it != _resultPath.begin();--it)
	{
		auto curGrid = *it;
		auto curDirection = curGrid - prevGrid;

		if (prevDiretion == curDirection)
		{
			_resultPath.erase(it + 1);
			continue;
		}
		prevDiretion = curDirection;
	}

	if (_resultPath.size() > 2 &&
		_resultPath[0] - _resultPath[1] == prevDiretion
		)
	{
		_resultPath.erase(_resultPath.begin() + 1);
	}

}