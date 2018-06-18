#include "Encoder.h"
#include <iostream>
#include <string>
#include "GridMap.h"
#include <vector>



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

string& Encoder::encodeAttack(int targetId)
{
	assert(_message[0] == 'a' || _message[0] == 'p');//追击或者攻击-------p:prepare attack
	char buff[5];
	sprintf(buff, "%02x", targetId);
	_message.append(buff);

	return _message;
}

string& Encoder::encodeCreate(int unitType, const Grid& createGrid)
{
	char buff[20];
	sprintf(buff, "%02x", unitType);//创造的兵种类
	sprintf(buff + 2, "%02x%02x", createGrid._x, createGrid._y);
	_message.append(buff);

	return _message;
}

string Encoder::encodeChat(string& chatMessage)
{
	_message.append(chatMessage);
	return _message;
}