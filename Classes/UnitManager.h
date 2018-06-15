#pragma once

#include "cocos2d.h"
#include <map>
#include "Unit.h"
#include "Data.h"//for MAX_PLAYER_NUM
#include "NotGay.h"
#include "Network/Client.h"
//#include "MainScene.h"

using std::map;
USING_NS_CC;

class Unit;
class ControlPanel;
class UnitManager:public Node {
	//test'
	friend class MainScene;
	friend class Unit;
	friend class SoldierCamp;
	friend class Factory;
public:
	Grid basePos[MAX_PLAYER_NUM] = {//基地坐标
		Grid(12,8),Grid(119,118),Grid(114,9),Grid(11,119),
	};

	Grid _basePos;//基地坐标
	void initAllBase();
private:
	int _playerNum = 0;

	int _playerId = 0;
	int _nextId = -1;//下一个兵的id
	vector<int> _selectId;//已经选中的id
	map<int, Unit* > _getUnitById;

	GridMap* _gridMap = nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	Client* _client = nullptr;
	ControlPanel* _controlPanel = nullptr;

private:
	bool initWithScene(MainScene* mainScene);
	bool init();
	void selectId(const Rect& rect);//做矩形框选的实际工作
	int getNextId();//下个id见			
	void abandonSelectedId();//清空选择							 
	bool isOurBro(int id);//检测id对应的Unit是不是自己人
public:

	static UnitManager* createWithScene(MainScene* mainScene);

	//CREATE_FUNC(UnitManager);

	void selectUnitByRect(const Rect& rect);

private:
	void selectBuilding(Unit* building);

	bool hasSelectIdNow();//判断是否已经有选中的id

	//这个是本地版本的create，产生unit的id并将信息传递给client
	void localCreateUnit(int type, const Point& createPoint);

	//制造延时效果并调用createUnit_
	void createUnit(int id, int type, const Grid& createGrid = Grid(6,6));
	//敌方和我方产生unit的逻辑都在这个函数完成
	void createUnit_(int id, int type, const Grid& createGrid = Grid(6, 6));

	void setPath(int id, GridMap::GridVector path);

	void selectUnitByPoint(const Point& point);

	void selectOneUnit(int id);

	//void updateUnitHp();

	void updateUnitState();

	void update(float delta);

	void deleteUnit(int id);
	void deleteUnit(Unit* unit);

	//检查unit是否死亡
	bool unitMayDead(Unit* attackee);

	int getIdByUnit(Unit* unit);
};