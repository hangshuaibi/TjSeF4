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
	_controlPanel = mainScene->_controlPanel;
	_notice = mainScene->_notice;
	_goldLabel = mainScene->_goldLabel;
	_powerLabel = mainScene->_powerLabel;

	assert(_client != nullptr);
	assert(_tiledMap != nullptr);
	assert(_gridMap != nullptr);
	assert(_controlPanel != nullptr);
	assert(_notice != nullptr);
	assert(_goldLabel != nullptr);
	assert(_powerLabel != nullptr);

	//_playerId = 0;//联网后此处得改
	//_nextId = _playerId + MAX_PLAYER_NUM;

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

void UnitManager::selectBuilding(Unit* building)
{
	Building* camp = dynamic_cast<Building*>(building);
	assert(camp != nullptr);
	
	camp->localCreateUnit();
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
				if (item.second->isBuilding())
				{
					selectBuilding(item.second);
					return;
				}

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
				
				if (item.second->isBuilding())//选中建筑
				{
					selectBuilding(item.second);
					return;
				}
				selectOneUnit(item.first);//选中一个单位，返回
				return;
			}
			else {//敌方单位，物伊
				//!!!-----暂未实现选中敌方的逻辑-----!!!//
				for (auto id : _selectId)
				{
					if (_getUnitById.count(id) == 0)
					{
						continue;
					}
					//auto unit = _getUnitById.at(id);
					
					//assert(unit != nullptr);
					Encoder encoder("p", id);
					std::string msg = encoder.encodeAttack(item.first);
					_client->sendMessage(msg);
					
				}

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

		auto path = pUnit->getPath(gridVector[--size]);//->>>>>>>>>测试完改回来
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
			pUnit->setDestination(gridVector[size]);

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
			rect.containsPoint(idWithUnit.second->getPosition())&&
			!idWithUnit.second->isBuilding())//建筑不被框选
		{
			selectOneUnit(idWithUnit.first);
		}
	}
}

void UnitManager::selectOneUnit(int id)
{
	_selectId.push_back(id);
	_getUnitById[id]->setOpacity(200);
}

void UnitManager::localCreateUnit(int type, const Point& point)
{
	//!!!--------加个判断不让point选取在障碍物之上--------!!!//
	auto createGrid = _gridMap->getGrid(point);
	auto nearValidGrid = _gridMap->findValidGridNear(createGrid);

	int id = getNextId();

	//根据type,id,position发信息给客户端
	Encoder encoder("c", id);
	std::string createMsg = encoder.encodeCreate(type, nearValidGrid);
	_client->sendMessage(createMsg);
}

void UnitManager::createUnit(int id, int type, const Grid& createGrid)
{
	auto sequence = Sequence::create(
		DelayTime::create(2.0f),
		CallFunc::create([=] {
		createUnit_(id, type, createGrid);//转发
	}),
		NULL
		);

	runAction(sequence);
}

void UnitManager::createUnit_(int id, int type, const Grid& createGrid)
{
	Unit* unit = nullptr;
	switch(type)
	{
	case Unit::Type::TANK: {
		unit = Tank::create(id);
		break;
	}
	case Unit::Type::SOILDER: {
		unit = Soldier::create(id);
		break;
	}
	case Unit::Type::SOLDIERCAMP: {
		unit = SoldierCamp::create(id);
		break;
	}
	case Unit::Type::BASE: {
		unit = Base::create(id);
		break;
	}
	case Unit::Type::FIGHTER: {
		unit = Fighter::create(id);
		break;
	}
	case Unit::Type::FACTORY: {
		unit = Factory::create(id);
		break;
	}
	case Unit::Type::ELECTRICITYFACTORY: {
		unit = ElectricityFactory::create(id);
		break;
	}
	case Unit::Type::MINE: {
		unit = Mine::create(id);
		break;
	}
	default:
		assert(0);//控制不应该到达这里
		break;
	}
	

	unit->setPosition(_gridMap->getPoint(createGrid));
	unit->addToMap(_gridMap, _tiledMap);//注意setPosition和addToMap的先后顺序，事关Building

	unit->setManager(this);
	unit->setId(id);

	_getUnitById.insert(std::make_pair(id, unit));

	unit->setProperties();
}

void UnitManager::deleteUnit(int id)
{
	if (_getUnitById.count(id) == 0)
	{
		return;
	}
	auto unit = _getUnitById.at(id);
	assert(unit != nullptr);

	if (unit->isBuilding())//建筑需要去除其在格点地图占位的逻辑
	{
		auto area = unit->getOccupiedArea();//获取占据区域
		_gridMap->setAreaValid(area);//将格点区域的bool逻辑设置为0

		if (unit->_id < 4 && unit->_id >= 0)//这...这是基地的坐标
		{
			if (unit->_id == _playerId)
			{
				notice("Loser!");//loser
				//切换场景
			}
			else if (++_loserNum == _playerNum - 1)
			{
				notice("Winner!");//winner
			}
		}
	}
	_tiledMap->removeChild(unit->_hp,true);
	_tiledMap->removeChild(unit, true);
	_getUnitById.erase(id);
	
}
void UnitManager::deleteUnit(Unit* unit)
{
	assert(unit != nullptr);

	int id = getIdByUnit(unit);
	if (id == -1)//不存在该Unit
	{
		return;
	}
	deleteUnit(id);
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
	//static bool startFlag = false;
	static bool imreadyFlag = false;

	string order = _client->getMessage();
	if (!_startFlag)//未开始
	{
		if (!imreadyFlag&&order[0] == 'I') {//Id(%d
			_playerId = order[3] - '1';
			_nextId = _playerId + MAX_PLAYER_NUM;
			imreadyFlag = true;
			_client->sendMessage("Client ready!");
			return;
		}
		else if (order[0] == 'S')//Start!
		{
			_playerNum = order[7] - '0';
			initAllBase();
			_startFlag = true;
			return;
		}
		return;
	}

	if (order[0] == 'n')//没有消息
	{
		return;
	}
	

	Decoder decoder(order);	
	int id = decoder.getId();

	switch (decoder.getType()) {
	case 't':
	case 'm': {
		auto path = decoder.decodePath();
		auto pUnit = _getUnitById[id];

		assert(pUnit != nullptr);	

		if (path.empty())
		{
			break;
		}
		pUnit->setGridPath(path);
		
		pUnit->schedule(schedule_selector(Unit::update));

		if(decoder.getType()=='m')
			pUnit->setState(Unit::MOVING);//根据traceFlag来确定状态的设置
		
		break;
	}
	case 'a': {
		//int id = decoder.getId();
		int targetId = decoder.decodeTargetId();
		/*Encoder encoder("a", id);
		auto msg = encoder.encodeAttack(targetId);
		msg.append("T");
		_client->sendMessage(msg);*/
		if (_getUnitById.count(id) != 1)break;
		auto atker = _getUnitById[id];
		auto atkee = _getUnitById[targetId];
		atker->shoot(atkee);
		if (targetId == _playerId)
		{
			notice(Notice::BASE_ATTACKED);
		}
		if (unitMayDead(atkee))//为真时单位在此函数被删除
		{
			atker->setTraceId(-1);
			atker->setState(Unit::WONDERING);
		}

		break;
	}
	case 'p': {
		//int id = decoder.getId();
		int targetId = decoder.decodeTargetId();
		if (_getUnitById.count(id) != 1)break;
		auto unit = _getUnitById[id];

		unit->setTraceId(targetId);
		unit->_gridPath.clear();//抽个函数
		unit->setState(Unit::TRACING);
		//>>>>>>>>>>>>>>>>>>>>>>send traceMessage 
		break;
	}
	case 'c': {
		int type = decoder.decodeCreateType();
		auto grid = decoder.decodeCreateGrid();
		createUnit(id, type, grid);
		break;
	}
	default:
		break;
	}
}

void UnitManager::update(float delta)
{
	updateLabel();//更新资源
	updateUnitState();
}

bool UnitManager::unitMayDead(Unit* attackee)
{
	if (attackee->_lifeValue < 0)
	{
		deleteUnit(attackee);
		return true;
	}
	return false;
}

int UnitManager::getIdByUnit(Unit* unit)
{
	for (auto& item : _getUnitById)
	{
		if (item.second == unit)
		{
			return item.first;
		}
	}

	return -1;//该单位已拜拜
}

void UnitManager::initAllBase()
{
	assert(_playerNum > 0);
	for (int id = 0;id < _playerNum;++id)
	{
		createUnit_(id, Unit::Type::BASE, basePos[id]);
	}

	assert(_playerId >= 0 && _playerId < MAX_PLAYER_NUM);
	_basePos = basePos[_playerId];
}

bool UnitManager::canCreate(int type)
{
	return getGold() > costGold[type] &&
		getElectricity() > costElectricity[type];
}

void UnitManager::costForCreate(int type)
{
	_gold -= costGold[type];
	_electricity -= costElectricity[type];

	for (int i = 0;i < 8;++i)
	{
		log("costGold: %d", costGold[i]);
	}
}

void UnitManager::notice(Notice note)
{
	static std::string msg[10] = {
		"No enough money!",
		"Base is attacked!",
		"Position is occupied!",
		"Please wait!",
		"Relax, game hasn't been started.",
	};
	
	_notice->setVisible(true);
	_notice->setString(msg[note]);
	//延时消失
	auto sequence = Sequence::create(
		DelayTime::create(2.0f),
		CallFunc::create([=] {
		_notice->setVisible(false);
	}),
		NULL
		);
	runAction(sequence);
}

void UnitManager::notice(std::string note)
{
	_notice->setColor(Color3B::RED);
	_notice->setString(note);
}

void UnitManager::updateLabel()
{
	static int timer = 0;
	static char gold[20] = "";//烫烫烫烫烫
	static char power[20] = "";
	static std::string gold_;
	static std::string power_;


	if (++timer == 60)
	{
		timer = 0;
		sprintf(gold, "%d", _gold);
		sprintf(power, "%d", _electricity);
		gold_.assign(gold);
		power_.assign(power);

		_goldLabel->setString(gold_);
		_powerLabel->setString(power_);
	}
}