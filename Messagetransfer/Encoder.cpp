#include "Encoder.h"
#include <iostream>
#include <string>
#include "GridMap.h"

Encoder::Encoder(const string& type, int id) :_type(type), _id(id)
{
	char* buff;
	sprintf(buff, "%2x", id);
	_message.append(_type);
	_message.append(buff);

}
string& Encoder::encodePath(GridVector& path)
{
	char* buff;
	int pointer = 0;
	for (auto point : path)
	{
		sprintf(buff + pointer, "%02x%02x", point._x, point._x);
		pointer += 4;
	}
	_message.append(buff);
	return _message;
}