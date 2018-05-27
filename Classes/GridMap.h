#pragma once
#include "cocos2d.h"
#include <vector>
#include <cassert>

using std::vector;
USING_NS_CC;

//格点，用于游戏中判断位置的占据
struct Grid {
	int _x;//横坐标，左上角为原点
	int _y;

	Grid(int x, int y):_x(x),_y(y){}
};

bool operator==(const Grid& lhs, const Grid& rhs);

Grid operator+(const Grid& lhs, const Grid&rhs);

Grid operator-(const Grid& lhs, const Grid&rhs);

//GridMap的坐标系与TiledMap不同
class GridMap:public cocos2d::Node {
private:
	int _mapWidth, _mapHeight;//格点地图的长宽
	int _gridWidth, _gridHeight;//每个格点的像素

	bool initWithTiledMap(TMXTiledMap* tileMap);//用该类型的map初始化

	vector<vector<int> > _isOccupied;//是否被占据，遵从effective stl中不用vector<bool>
	vector<vector<int> > _occupiedId;//占据对应位置的id
	Vec2 _pointOffset;//点中心到所取点的偏移，这是由于int类型造成的

	void occupyGrid(int id, const Grid& g);//占领格点的实际工作者
public:
	static GridMap* create(TMXTiledMap* tileMap);
	bool isGridInMap(const Grid& g);//判断是否在地图内
	//bool isPointOccupied(const Point& p);
	bool isGridOccupied(const Grid& g);//若被占领返回1

	bool tryToOccupyGrid(int id, const Grid& g);//id单位尝试占领某位置，占领成功返回true
	void leaveGrid(int id, const Grid& g);//id离开当前格点
	
	typedef vector<Grid> GridVector;

	//搜寻P点附近空闲的点，返回一个大小为size的数组
	GridVector findValidGridNear(const Grid& p, int size);
	Grid findValidGridNear(const Grid& p);//搜寻P点附近空闲的点

	Point getPoint(const Grid& g);//坐标变换，注意两坐标轴的方向，已调整偏移量
	Grid getGrid(const Point& p);

};

