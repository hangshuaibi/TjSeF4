#include "UnitManager.h"
#include <utility>//for make_pair
//#include "GridMap.h"
#include "RealUnit.h"
#include "MainScene.h"
#include "Messagetransfer/Encoder.h"
#include "Messagetransfer/Decoder.h"
class MainScene;

UnitManager* UnitManager::createWithScene(MainScene* mainScene)
{
	UnitManager* ret = new UnitManager();
	if (ret&&ret->initWithScene(mainScene))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool UnitManager::initWithScene(MainScene* mainScene)
{
	_client = mainScene->_client;
	_tiledMap = mainScene->_tiledMap;
	_gridMap = mainScene->_gridMap;

	assert(_client != nullptr);
	assert(_tiledMap != nullptr);
	assert(_gridMap != nullptr);

	_playerId = 0;//联网后此处得改
	_nextId = _playerId;

	return true;
}

//弃用
bool UnitManager::init()
{
	_playerId = 0;
	_nextId = _playerId;

	return true;
}

bool UnitManager::hasSelectIdNow()
{
	return !_selectId.empty();
}

void UnitManager::selectUnitByRect(const Rect& rect)
{
	assert(_gridMap != nullptr);

	if (rect.getMaxX()==rect.getMinX())//点击而不是框选
	{
		selectUnitByPoint(Point(rect.getMinX(), rect.getMinY()));
		return;
	}

	if (hasSelectIdNow())
	{
		//重新框选
		abandonSelectedId();
	}

	//无选中单位时框选
	selectId(rect);
}

void UnitManager::selectUnitByPoint(const Point& point)
{
	//如果没有选中的单位
	if (!hasSelectIdNow())
	{
		for (auto &item : _getUnitById)
		{
			if (isOurBro(item.first) &&
				item.second->getBoundingBox().containsPoint(point))
			{
				selectOneUnit(item.first);//选中一个单位，返回
				return;
			}
		}
	//当前没有选中并且此次单击的点不是自己的单位，直接返回
		return;
	}

	//控制到达这里一定有单位已经被选中
	for (auto& item : _getUnitById)
	{
		//有敌方或者我方的单位被选中
		if (item.second->getBoundingBox().containsPoint(point))
		{
			if (isOurBro(item.first))
			{
				abandonSelectedId();
				selectOneUnit(item.first);//选中一个单位，返回
				return;
			}
			else {//敌方单位，物伊
				//!!!-----暂未实现选中敌方的逻辑-----!!!//

				return;
			}
		}
	}


	//到达这里，有单位在之前的框选被选中，让被选中的到达这个点
	auto destPoint = point;//dest grid
	auto destGrid = _gridMap->getGrid(destPoint);

	//find valid grid around dest depend on size
	int size = _selectId.size();
	GridMap::GridVector gridVector = _gridMap->findValidGridsNear(destGrid, size);

	//确认返回了足够大的数组
	assert(gridVector.size() == size);

	for (int id : _selectId)
	{
		auto pUnit = _getUnitById[id];
		if (pUnit == nullptr)
		{
			continue;//单位可能已经被删除
		}

		auto path = pUnit->getPath(gridVector[size - 1]);//->>>>>>>>>测试完改回来
		Encoder encoder("m", id);
		string message = encoder.encodePath(path);
		_client->sendMessage(message);//id,path

		if(0)
		{
			string order = _client->getMessage();
			while(order[0] == 'y' || order[0] == 'c'||order[0]=='n') {
				order = _client->getMessage();
			}
			Decoder decoder(order);
			Encoder test("m", decoder.getId());
			auto dpath = decoder.decodePath();
			string testmsg = test.encodePath(dpath);
			testmsg.push_back('T');

			_client->sendMessage(testmsg);
		}

		if(0)//还没加网络哦
		{
			pUnit->setState(Unit::WONDERING);
			pUnit->setDestination(gridVector[--size]);

			pUnit->findPath();
			pUnit->schedule(schedule_selector(Unit::update));

			pUnit->setState(Unit::MOVING);
		}
	}

	
}

void UnitManager::selectId(const Rect& rect)
{
	for (const auto&idWithUnit : _getUnitById)
	{
		if (isOurBro(idWithUnit.first) && //自己人才享有控制权
			rect.containsPoint(idWithUnit.second->getPosition()))
		{
			selectOneUnit(idWithUnit.first);
		}
	}
}

void UnitManager::selectOneUnit(int id)
{
	_selectId.push_back(id);
	_getUnitById[id]->setOpacity(100);
}

void UnitManager::localCreateUnit(int/* type*/, const Point& point)
{
	//!!!--------加个判断不让point选取在障碍物之上--------!!!//
	auto createGrid = _gridMap->getGrid(point);
	auto nearValidGrid = _gridMap->findValidGridNear(createGrid);

	int id = getNextId();

	//根据type,id,position发信息给客户端
	_client->sendMessage("create a unit");
}

void UnitManager::createUnit(int id, int type, const Grid& createGrid)
{
	//根据type和playerId选取相应图片
	auto unit = Tank::create("tank.png");

	unit->addToMap(_gridMap, _tiledMap);
	unit->setPosition(_gridMap->getPoint(createGrid));
	unit->setManager(this);

	_getUnitById.insert(std::make_pair(id, unit));

	unit->setProperties();
}

int UnitManager::getNextId()
{
	int retId = _nextId;

	_nextId += MAX_PLAYER_NUM;

	return retId;
}

void UnitManager::abandonSelectedId()
{
	for (int id : _selectId)
	{
		//原始透明度是255
		_getUnitById[id]->setOpacity(255);
	}

	_selectId.clear();
}

bool UnitManager::isOurBro(int id)
{
	return id%MAX_PLAYER_NUM == _playerId;
}

void UnitManager::setPath(int id, GridMap::GridVector path)
{
	auto unit = _getUnitById[id];
	assert(unit != nullptr);

	unit->_gridPath = path;
}

void UnitManager::updateUnitState()
{
	string order = _client->getMessage();
	while (order[0] == 'y' || order[0] == 'c') {
		order = _client->getMessage();
	}
	if (order[0] == 'n')
	{
		return;
	}
	
	Decoder decoder(order);	
	
	switch (decoder.getType()) {
	case 'm': {
		int id = decoder.getId();
		auto path = decoder.decodePath();
		auto pUnit = _getUnitById[id];

		assert(pUnit != nullptr);	

		pUnit->setState(Unit::WONDERING);
		pUnit->setGridPath(path);
		
		pUnit->schedule(schedule_selector(Unit::update));
		pUnit->setState(Unit::MOVING);

		break;
	}
			 
	default:
		break;
	}
}

void UnitManager::update(float delta)
{
	updateUnitState();
}