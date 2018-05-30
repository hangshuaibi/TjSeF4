//
//  BattleSceneScene.cpp
//  AirfightBattleScene
//
//  Created by Jason on 14-5-8.
//
//

#include "BattleScene.h"
#include "ui/CocosGUI.h"
//#include "FKSprite.h"
USING_NS_CC;
using namespace ui;

// 不同敌机的tag
static int E0TAG = 10;
static int E1TAG = 11;
static int E2TAG = 12;
static int BOSSTAG = 100;

Scene* BattleScene::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = BattleScene::create();
	// 将BattleScene层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

// on "init" you need to initialize your instance
bool BattleScene::init()
{
	// 调用父类的init方法
	if (!Layer::init())
	{
		return false;
	}
	// 获得设备可见视图大小
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 获取屏幕宽度和高度
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;


	_battleMap = TMXTiledMap::create("map/LostTemple.tmx");
	_battleMap->setAnchorPoint(Vec2(0, 0));
	addChild(_battleMap, 0);

	return true;
}

void BattleScene::onEnter() {
	Layer::onEnter();

	}

// 回调更新函数，该函数每一帧都会调用
void BattleScene::update(float delta) {
	
}


/*************************敌机相关函数*****************************************/



