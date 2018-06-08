#pragma once

#include "cocos2d.h"
#include "iostream"
#include "GridMap.h"
#include <string>
#include <vector>
#include "Data.h"
using namespace std;
typedef vector<Grid> GridVector;

//全局变量，存储解码后的格点数组


class Decoder {
private:

	GridVector _path;

	//从服务器端取到的信息
	string _message;

	//信息表示的操作类型
	string _type;

	//信息中士兵的的id
	int _id;
public:

	//用取到的信息构造Decoder
	Decoder(const string& message);

	//从信息中解码出路径
	GridVector& decodePath();

	//从信息中解码出要攻击对象的Id
	int decodeAttachId();

	//从信息中解码出要生产的建筑或士兵
	string decoderProduce();

	//返回信息中士兵的id
	int getId();


	//返回操作类型
	string getType();

};
