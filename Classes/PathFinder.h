#pragma once

#include "cocos2d.h"
#include <vector>
#include <algorithm>//for heap
#include "GridMap.h"

using std::vector;
USING_NS_CC;

static const int DIRECTIONS[8][2] =
{ { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 },{ 0,1 },{ 1,-1 },{ 1,0 },{ 1,1 } };

class PathFinder {
	static const int DISTANCE0 = 10;
	static const int DISTANCE1 = 14;

	static const int DIRECTION_NUM = 8;
	

private:
	struct grid {//作为PathFinder的内部实现
		static const int MAX_VALUE = (1 << 31) - 1;

		grid(int x = -1, int y = -1):
			x(x),y(y),state(UNOCCUPIED),fValue(MAX_VALUE),
			gValue(MAX_VALUE),hValue(MAX_VALUE),
			parent(nullptr){}


		//横纵坐标
		int x, y;

		enum State {
			INOPENLIST = 0,
			INCLOSELIST,
			START,
			END,
			OCCUPIED,
			UNOCCUPIED
		};

		State state;

		int fValue;//衡量该点的总权值
		int gValue;//起点到该点的估计权值
		int hValue;//该点到终点的估计权值，曼哈顿距离作为启发函数

		grid* parent;

	
	};

	int _mapWidth, _mapHeight;

	//const vector<vector<int> >& _gridTable;//描述格点地图是否被占据
	vector<Grid> _resultPath;

	grid *_start, *_end;

	vector<vector<grid> > _gridTable;//格点表

	
	vector<grid*> _openList;//maybe use heap to make PA better

public:
	vector<Grid> getPath();
	PathFinder(const vector<vector<int> > &gridTable,
		const Grid& start, const Grid& end);
	bool searchPath();
	void generatePath();

private:
	bool isGridValid(const grid* g);

	void setState(grid* g, grid::State state);

	grid* nextToSearch();

	bool isInOpenList(const grid* g);
	bool isInCloseList(const grid* g);
	//remove from open list and make the state to be INCLOSELIST
	void removeFromOpenList(grid* g/**/);

	//计算前驱点到当前点的估计值
	int calculateGValue(const grid* parent, const grid* cur);
	//当前点到终点的估计值
	int calculateHValue(const grid* cur, const grid* end);

	void checkSurroundGrid(const grid* pg);

	/*想象由四个格点组成一个正方形，在此规定若对角的两个点若有一个为OCCUPIED，
	剩下的两个点无法连接，返回false*/
	bool checkCorner(const grid* rhs, const grid* lhs);
};