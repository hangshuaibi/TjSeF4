#pragma once
#include "cocos2d.h"
#include "GridMap.h"
#include <map>
#include "NotGay.h"


using std::map;
using std::string;
USING_NS_CC;

class UnitManager;
class Building :public Sprite {
	friend class UnitManager;
public:

	Building()
	{}

	virtual int getGold()=0;

	virtual int getElectricity()=0;

	void addToMap(GridMap* gridMap, TMXTiledMap* _tiledMap);

	GridMap* _gridMap = nullptr;

	virtual bool isgold() = 0;

	UnitManager* _unitManager = nullptr;



};

class Barracks:public Building{
public:
	virtual int getGold()
	{
		return -40;
	}

	virtual int getElectricity()
	{
		return -5;
	}

	virtual bool isgold()
	{
		return 0;
	}


	static Barracks* Barracks::create();
	

	int id = 0;
};

class Warfactory :public Building {
public:
	virtual int getGold()
	{
		return -60;
	}

	virtual int getElectricity()
	{
		return -8;
	}

	virtual bool isgold()
	{
		return 0;
	}


	static Warfactory* Warfactory::create();


};

class Storage :public Building {
public:
	virtual int getGold()
	{
		return -20;
	}

	int id = 0;

	virtual int getElectricity()
	{
		return 30;
	}

	virtual bool isgold()
	{
		return 0;
	}



	static Storage* Storage::create();

};

class Producer :public Building {
public:

	virtual bool isgold()
	{
		return 1;
	}




	
	virtual int getGold()
	{
		return -20;
	}

	virtual int getElectricity()
	{
		return -3;
	}

	int id = 0;

	static Producer* Producer::create();

};



