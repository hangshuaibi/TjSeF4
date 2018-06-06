#pragma once

#include "cocos2d.h"
#include "iostream"
#include "GridMap.h"
#include <string>
#include <vector>
#include "Data.h"
using namespace std;

//格点数组
typedef vector<Grid> GridVector;

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
	string& encodePath(GridVector& path);

};
