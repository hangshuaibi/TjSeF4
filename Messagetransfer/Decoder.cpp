#include "Decoder.h"

#include "GridMap.h"
#include "Data.h"
const char MOVE = 'm';

//全局变量，存储解码后的格点数组


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