#pragma once

#include "cocos2d.h"
#include "iostream"
#include "GridMap.h"
#include <string>
#include <vector>
#include "Data.h"
using namespace std;

//定义move操作类型为'm'
const char MOVE = 'm';

//定义攻击操作类型为'p'
const char ATTACK = 'a';

//定义生产兵或者生产建筑物的操作码为'p'
const char PRODUCE = 'p';

//定义不同生产物(建筑或士兵的指定码）


const string ELECTRICITY = "el";     //定义电厂

const string MINE = "mi";           //定义矿场

const string CAMP = "ca";           //定义兵营

const string FACTORY = "fa";         //定义战车工厂

const string DOG = "do";             //定义警犬

const string SOLDIER = "so";         //定义大兵

const string TANK = "ta";            //定义坦克

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
	Encoder(const string& type, int id = 0);        //为生产类型提供默认id

	//Encoder将格点数组类转换成要发送的字符串
	string& encodePath(GridMap::GridVector& path);


	//将要攻击的士兵的Id，执行攻击的士兵的id, 操作类型转换成字符串，并返回
	string& encodeAttack(int id);


	//将要生产的士兵或建筑物等的类型转换成字符串，并返回。（produceType为每种生产的东西的const 字符串的代号，如电厂为"el"）
	string& encodeProduce(const string& produceType);
};
