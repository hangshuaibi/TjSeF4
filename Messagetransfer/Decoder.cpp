#include "Decoder.h"

#include "GridMap.h"
#include "Data.h"


const char MOVE = 'm';
const char ATTACK = 'a';
const char CREATE = 'c';


Decoder::Decoder(const string& message) :_message(message)
{
	//string _type = _message.substr(0, 1);//局部变量覆盖私有变量
	_type = _message.substr(0, 1);
	string id = _message.substr(1, 2);
	sscanf(id.c_str(), "%x", &_id);
}
int Decoder::getId()
{
	return _id;
}
GridVector Decoder::decodePath()
{
	assert(_type[0] == MOVE);
	Grid point;
	int beginPos = 3;

	_path.clear();//清空数组
	while (beginPos < _message.length() - 1)
	{
		string temp = _message.substr(beginPos, 2);
		//int x, y;
		sscanf(temp.c_str(), "%x", &point._x);
		beginPos += 2;
		temp = _message.substr(beginPos, 2);
		sscanf(temp.c_str(), "%x", &point._y);
		_path.emplace_back(point);
		beginPos += 2;
	}
	return _path;
}

char Decoder::getType()
{
	return _message[0];
}

int Decoder::decodeTargetId()
{
	assert(_message[0] == ATTACK);

	int targetId = -1;
	string temp = _message.substr(3, 2);
	sscanf(temp.c_str, "%x", &targetId);
	assert(targetId != -1);//读取成功
	
	return targetId;
}

int Decoder::decodeCreateType()
{
	assert(_message[0] == CREATE);

	int unitType = -1;
	string temp = _message.substr(3, 2);
	sscanf(temp.c_str, "%x", &unitType);
	assert(unitType != -1);

	return unitType;
}

Grid Decoder::decodeCreateGrid()
{
	assert(_message[0] == CREATE);

	Grid ret;
	string temp = _message.substr(5, 2);
	sscanf(temp.c_str, "%x", &ret._x);
	temp = _message.substr(7, 2);
	sscanf(temp.c_str, "%x", &ret._y);

	assert(ret._x != -1 && ret._y != -1);
	
	return ret;
}