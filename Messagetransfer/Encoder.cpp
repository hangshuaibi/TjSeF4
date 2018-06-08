#include "Encoder.h"
#include <iostream>
#include <string>
#include "GridMap.h"

class Encoder;
Encoder::Encoder(const std::string& type, int id) :_type(type), _id(id)
{
	char buff[20];
	sprintf(buff, "%2x", id);//---
	_message.append(_type);
	_message.append(buff);

}
std::string& Encoder::encodePath(GridMap::GridVector& path)
{
	char buff[1024];
	int pointer = 0;
	for (auto point : path)
	{
		sprintf(buff + pointer, "%02x%02x", point._x, point._y);//
		pointer += 4;
	}
	_message.append(buff);
	assert(_message.size() < 512);
	return _message;
}