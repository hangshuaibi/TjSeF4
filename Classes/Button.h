#pragma once

/*为了实现创建建筑时的拖动效果实现的一个类*/

#include <string>
#include "cocos2d.h"
#include "Unit.h"


USING_NS_CC;
typedef  typename Unit::Type Type;

class UnitManager;
class BButton :public Sprite {
private:
	//副本，用于拖动时的显示
	Sprite * _buttonCopy = nullptr;
	Type _type;
	UnitManager* _unitManager = nullptr;

public:
	static BButton* create(MainScene* scene, Type type, std::string filename);

	bool init(MainScene* scene, Type type, std::string filename);

	Sprite* copy()
	{
		return _buttonCopy;
	}

	bool isTouched(Touch* touch);

	void onPress();//将副本设置为可见
	void onRelease();//释放时生产单位
					 //void setUnitManager(UnitManager* unitManager);
};