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

enum Notice {
	NO_ENOUGH_MONEY=0,//金钱不足
	BASE_ATTACKED,//基地遭受攻击
	OCCUPIED_POSITION,//地方被占据（生产
	PLEASE_WAIT,//生产兵需要cd
	WAIT_FOR_START,//不要猴急
};

class Unit;
class ControlPanel;
class UnitManager:public Node {
	//test'
	friend class MainScene;
	friend class Unit;
	friend class SoldierCamp;
	friend class Factory;
	friend class Mine;
	friend class ElectricityFactory;
	friend class BButton;
private:
	Grid basePos[MAX_PLAYER_NUM] = {//基地坐标
		Grid(12,8),Grid(119,118),Grid(114,9),Grid(11,119),
	};
private:
	//每个单位生产消耗的金钱
	int costGold[10] = {
		200,100,300,500,700,0,300,400
	};
	//每个单位生产消耗的电力
	int costElectricity[10] = {
		100,50,200,300,400,0,100,200
	};
	bool _startFlag = false;
public:
	bool isStart() { return _startFlag; }

	bool canCreate(int type);
	//肯定要花钱啊
	void costForCreate(int type);
//private:
	Grid _basePos;//基地坐标
	void initAllBase();
private:
	int _gold = 10000;
	int _electricity = 10000;
	Label* _goldLabel = nullptr;
	Label* _powerLabel = nullptr;

	int _playerNum = 0;
	int _loserNum = 0;

	int _playerId = 0;
	int _nextId = -1;//下一个兵的id
	vector<int> _selectId;//已经选中的id
	map<int, Unit* > _getUnitById;

	GridMap* _gridMap = nullptr;
	TMXTiledMap* _tiledMap = nullptr;
	Client* _client = nullptr;
	ControlPanel* _controlPanel = nullptr;
	Label* _notice = nullptr;

private:
	bool initWithScene(MainScene* mainScene);

	bool init();

	void selectId(const Rect& rect);//做矩形框选的实际工作

	int getNextId();//下个id见			

	void abandonSelectedId();//清空选择			

	bool isOurBro(int id);//检测id对应的Unit是不是自己人

public:
	//获得当前的金钱总数
	int getGold()
	{
		return _gold;
	}
	//获得当前的电力总数
	int getElectricity()
	{
		return _electricity;
	}

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

	void updateLabel();

	void updateUnitState();

	void update(float delta);

	void deleteUnit(int id);
	void deleteUnit(Unit* unit);

	//检查unit是否死亡
	bool unitMayDead(Unit* attackee);

	int getIdByUnit(Unit* unit);
public:
	void notice(Notice note);
	void notice(std::string);
};