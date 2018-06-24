#pragma once

/*这是子弹类，主要是为了保证发射子弹单位删除后血量减少逻辑的正常
（当然有很多种避免的办法，开个类写多少有点小题大做），为的是留个坑
加追踪效果和爆炸动画（改完bug再说（逃
突然发现好像不得不写了......*/

#include "cocos2d.h"
#include "Unit.h"

USING_NS_CC;

class Bullet :public Sprite {
public:
	static Bullet* create(Unit* atker, int targetId);

	//void update(float)override;

	void runBullet();

private:
	//销毁子弹
	void destroy();

	bool init(Unit* atker, int targetId);


	Vec2 _start, _end;
	int _effect = 0;
	int _atkeeId = -1;
	float _speed = 96.f;
	Node* _map = nullptr;
	UnitManager* _unitManager = nullptr;
};