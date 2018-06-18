#pragma once

#include "cocos2d.h"
#include "iostream"
#include "GridMap.h"
#include <string>
#include <vector>
#include "Data.h"
using namespace std;


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
	string& encodePath(GridMap::GridVector& path);


	//将要攻击的士兵的Id，执行攻击的士兵的id, 操作类型转换成字符串，并返回
	string& encodeAttack(int targetId);


	//将要生产的士兵或建筑物等的类型, 转换成字符串，并返回。（produceType为每种生产的东西的const 字符串的代号，如电厂为"el"）
	string& encodeCreate(int unitType, const Grid& createGrid);


	//将聊天的信息解码到字符串中
	string encodeChat(string& chatMessage);
};
