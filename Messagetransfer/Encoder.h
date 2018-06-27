#pragma once

/*这是编码类，通过制定一定的规则实现将玩家操作编码成可发送的信息
并由UnitManager经过客户端发向服务端*/

#include "cocos2d.h"
#include "iostream"
#include "GridMap.h"
#include <string>
#include <vector>
#include "Data.h"
using std::string;

//typedef GridMap::GridVector GridVector;

class Encoder {
private:

	//操作类型
	string _type;

	//兵的id
	int _id;

	//要发送的string类信息
	string _message;
public:

	//首先用操作类型和士兵id构造Encoder
	Encoder(const string& type, int id);

	//Encoder将格点数组类转换成要发送的字符串
	const string& encodePath(const GridMap::GridVector& path);

	const string& encodeAttack(int targetId);

	const string& encodeCreate(int unitType, const Grid& createGrid);

	const string& encodeChat(string& chatMessage);
};
