#include "Encoder.h"
#include <iostream>
#include <string>
#include "GridMap.h"




Encoder::Encoder(const string& type, int id) :_type(type), _id(id)
{
	char buff[20];
	sprintf(buff, "%02x", id);
	_message.append(_type);
	_message.append(buff);

}
string& Encoder::encodePath(GridMap::GridVector& path)
{
	char buff[1024];
	int pointer = 0;
	for (auto point : path)
	{
		sprintf(buff + pointer, "%02x%02x", point._x, point._y);
		pointer += 4;
	}
	_message.append(buff);
	assert(_message.size() < 512);
	return _message;
}

string& Encoder::encodeAttack(int id)
{
	char buff[20];
	sprintf(buff, "%02x", id);
	_message.append(buff);
	return _message;
}

string& Encoder::encodeProduce(const string& produceType)
{
	_message = _message.substr(0, 1);   //生产类型的消息不需要士兵的id；
	_message.append(produceType);
	return _message;
}