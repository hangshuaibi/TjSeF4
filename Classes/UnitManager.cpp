#include "UnitManager.h"
#include <utility>//for make_pair
//#include "GridMap.h"
#include "RealUnit.h"
#include "MainScene.h"
#include "Messagetransfer/Encoder.h"
#include "Messagetransfer/Decoder.h"
#include "Scenes/EndScene.h"
#include "Scenes//ServerOrNot.h"

static bool imreadyFlag = false;

std::mutex lock;
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

	//----------//
	_displayValueLabel[0] = mainScene->_displayValueLabel[0];
	_displayValueLabel[1] = mainScene->_displayValueLabel[1];

	assert(_displayValueLabel[0] != nullptr);
	assert(_displayValueLabel[1] != nullptr);
	//----------//

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
		if (_getUnitById.count(id) != 1)
		{
			continue;
		}
		auto pUnit = _getUnitById[id];
		//if (pUnit == nullptr)
		{
			//continue;//单位可能已经被删除
		}

		auto path = pUnit->getPath(gridVector[--size]);//->>>>>>>>>测试完改回来
		Encoder encoder("m", id);
		string message = encoder.encodePath(path);
		_client->sendMessage(message);//id,path

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
	//auto nearValidGrid = _gridMap->findValidGridNear(createGrid);

	int id = getNextId();

	//根据type,id,position发信息给客户端
	Encoder encoder("c", id);
	std::string createMsg = encoder.encodeCreate(type, createGrid);
	_client->sendMessage(createMsg);
}

void UnitManager::createUnit(int id, int type, const Grid& createGrid)
{
	if (type > Unit::Type::FIGHTER)
	{
		//建筑播放动画
		displayAnimate(_gridMap->getPoint(createGrid));
	}
	auto unit = createUnit_(id, type, createGrid);//转发
	unit->setVisible(false);

	auto sequence = Sequence::create(
		DelayTime::create(2.0f),
		CallFunc::create([=] {
		unit->setVisible(true);
		unit->_hp->setVisible(true);
		_getUnitById.insert(std::make_pair(id, unit));//延后insert防止隐藏时响应点击
	}),
		NULL
		);

	runAction(sequence);
}

Unit* UnitManager::createUnit_(int id, int type, const Grid& createGrid)
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
		if (id >= _playerId + MAX_PLAYER_NUM * 63)
		{
			throw("take a rest, your unit number is beyond 64!");
		}
		assert(0);//控制不应该到达这里
		break;
	}
	

	unit->setPosition(_gridMap->getPoint(createGrid));
	unit->addToMap(_gridMap, _tiledMap);//注意setPosition和addToMap的先后顺序，事关Building

	unit->setManager(this);
	unit->setId(id);


	unit->setProperties();
	unit->_hp->setVisible(false);

	return unit;
}

void UnitManager::displayAnimate(Point pos)
{
	auto creation = AnimationCache::getInstance()->getAnimation("create");

	auto animate = Animate::create(creation);
	//auto repeat = Repeat::create(animate, 2.f / 2 + 1);

	auto animationSprite = Sprite::createWithSpriteFrameName("animate1.png");
	_tiledMap->addChild(animationSprite, 10);
	animationSprite->setPosition(pos);


	Sequence* seq = Sequence::create(animate, RemoveSelf::create(true), NULL);
	animationSprite->runAction(seq);
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
				auto transiton = TransitionSlideInL::create(2.f, EndScene::createScene(false));
				Director::getInstance()->replaceScene(transiton);
			}
			else if (++_loserNum == _playerNum - 1)//无敌是多么多么地寂寞
			{
				notice("Winner!");//winner
				auto transiton = TransitionSlideInL::create(2.f, EndScene::createScene(true));
				Director::getInstance()->replaceScene(transiton);
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
		if (_getUnitById.count(id) != 1)
		{
			continue;
		}
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
	if (_getUnitById.count(id) != 1)
	{
		return;
	}
	auto unit = _getUnitById[id];
	assert(unit != nullptr);

	unit->_gridPath = path;
}

void UnitManager::updateUnitState()
{
	//static bool startFlag = false;
	//static bool imreadyFlag = false;
	
	//if (imreadyFlag)
	{
		//Sleep(100);
	}
	//lock.lock();
	string order = _client->getMessage();
	//assert(order[0] != 'S');
	if (!_startFlag)//未开始
	{
		if (!imreadyFlag&&order[0] == 'I') {//Id(%d
			_playerId = order[3] - '1';
			_nextId = _playerId + MAX_PLAYER_NUM;
			imreadyFlag = true;
			//lock.unlock();

			order = _client->getMessage();
			if (order[0] == 'S')//Start!
			{
				_playerNum = order[7] - '0';
				initAllBase();
				//ServerOrNot::getClient()->sendMessage();
				ServerOrNot::getClient()->release();//断开与远端的连接
				_startFlag = true;
				notice(Notice::GAME_START);
				
				//return;
			}
			return;
		}
		else if (order[0] == 'S')//Start!
		{
			assert(0);
			_playerNum = order[7] - '0';
			initAllBase();
			_startFlag = true;
			notice(Notice::GAME_START);
			return;
		}
		return;
	}
	//assert(0);
	if (order[0] == 'n')//没有消息
	{
		return;
	}


	Decoder decoder(order);
	int id = decoder.getId();
	if (_getUnitById.count(id) == 0
		&&decoder.getType() != 'c')
	{
		return;
	}

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

		if (decoder.getType() == 'm')
			pUnit->setState(Unit::MOVING);//根据traceFlag来确定状态的设置

		break;
	}
	case 'a': {
		int targetId = decoder.decodeTargetId();
		if (_getUnitById.count(targetId) != 1)
		{
			break;
		}

		auto atker = _getUnitById[id];
		auto atkee = _getUnitById[targetId];
		atker->shoot(atkee);
		if (targetId == _playerId)
		{
			notice(Notice::BASE_ATTACKED);
		}
		if (unitMayDead(atkee))//为真时单位在此函数被删除
		{
			if (_getUnitById.count(id) != 1)break;
			atker->setTraceId(-1);
			atker->setState(Unit::WONDERING);
		}

		break;
	}
	case 'p': {
		int targetId = decoder.decodeTargetId();
		if (_getUnitById.count(id) != 1)break;
		auto unit = _getUnitById[id];

		unit->setTraceId(targetId);
		unit->_gridPath.clear();//抽个函数
		unit->setState(Unit::TRACING);
		//unit->_timer = 59;//60-1
		
		break;
	}
	case 'c': {
		int type = decoder.decodeCreateType();
		auto grid = decoder.decodeCreateGrid();
		createUnit(id, type, grid);

		break;
	}
	case 'g': {//gossip
		std::string temp(decoder.decodeChat());
		
		_displayValueLabel[isLabelFree[0]^1]->setString(temp);
		isLabelFree[0] ^= 1;//异或
		isLabelFree[1] ^= 1;

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
		attackee->unschedule(schedule_selector(Unit::update));
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
		createUnit(id, Unit::Type::BASE, basePos[id]);
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
}

void UnitManager::notice(Notice note)
{
	static std::string msg[10] = {
		"No enough money!",
		"Base is attacked!",
		"Position is occupied!",
		"Please wait!",
		"Relax, game hasn't been started.",
		"Start now!"
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