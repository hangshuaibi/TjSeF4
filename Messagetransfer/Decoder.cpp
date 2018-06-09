#include "Decoder.h"
#include <iostream>
#include <string>
#include "GridMap.h"


//定义move操作类型为'm'
const char MOVE = 'm';

//定义攻击操作类型为'p'
const char ATTACK = 'a';

//定义生产兵或者生产建筑物的操作码为'p'
const char PRODUCE = 'p';

//定义不同生产物(建筑或士兵的指定码）


const string ELECTRICITY = "el";     //定义电厂

const string MINE = "mi";           //定义矿场

const string CAMP = "ca";           //定义兵营

const string FACTORY = "fa";         //定义战车工厂

const string DOG = "do";             //定义警犬

const string SOLDIER = "so";         //定义大兵

const string TANK = "ta";            //定义坦克

Decoder::Decoder(const string& message) :_message(message)
{
		_type = _message.substr(0, 1);
		string id = _message.substr(1, 2);
		sscanf(id.c_str(), "%x", &_id);
}
int Decoder::getId()
{
	return _id;
}


GridVector& Decoder::decodePath()
{
	assert(_type[0] == MOVE);
	Grid point;
	int beginPos = 3;

	_path.clear();// 清空数组
	while (beginPos < _message.length() - 1)
	{
		string temp = _message.substr(beginPos, 2);
		int x, y;
		sscanf(temp.c_str(), "%x", &point._x);
		beginPos += 2;
		temp = _message.substr(beginPos, 2);
		sscanf(temp.c_str(), "%x", &point._y);
		_path.emplace_back(point);
		beginPos += 2;
	}
	return _path;
}

int Decoder::decodeAttackId()
{
	assert(_type[0] == ATTACK);
	int id;
	string temp = _message.substr(3, 2);
	sscanf(temp.c_str(), "%x", &id);
	return id;
}

string& Decoder::decodeProduceType()
{
	assert(_type[0] == PRODUCE);

	_produceType = _message.substr(3, 2);
	return _produceType;
}
char Decoder::getType()
{
	return _message[0];
}