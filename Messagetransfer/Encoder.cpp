#include "Encoder.h"
#include <iostream>
#include <string>
#include "GridMap.h"

Encoder::Encoder(const string& type, int id) :_type(type), _id(id)
{
	char* buff = new char[3];
	sprintf(buff, "%2x", id);
	_message.append(_type);
	_message.append(buff);
	delete[] buff;

}
string& Encoder::encodePath(GridVector& path)
{
	int len = path.size();
	char* buff = new char[4 * len + 1];
	int pointer = 0;
	for (auto point : path)
	{
		sprintf(buff + pointer, "%02x%02x", point._x, point._y);
		pointer += 4;
	}
	_message.append(buff);
	delete[] buff;
	return _message;
}

string& Encoder::encodeAttach(int id)
{
	char* buff = new char[3];
	sprintf(buff, "%02x", id);
	_message.append(buff);
	delete[] buff;
	return _message;
}

string& Encoder::encodeProduce(string& produceType)
{
	_message.append(produceType);
	return _message;
}