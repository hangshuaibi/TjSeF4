#include "MainScene.h"
#include "cocos2d.h"

USING_NS_CC;

MainScene* MainScene::create()
{
	MainScene* mainScene = new MainScene();
	if (mainScene&&mainScene->init())
	{
		mainScene->autorelease();
		return mainScene;
	}

	log("MainScene::create error\n");

	CC_SAFE_DELETE(mainScene);
	return nullptr;
}

MainScene* MainScene::createScene()
{
	return MainScene::create();
}

bool MainScene::init()
{
	//父类函数
	if (!Scene::init())
	{
		return false;
	}

	//暂时用lostTemple.tmx
	_tiledMap = TMXTiledMap::create("Map/LostTemple.tmx");

	_tiledMap->setAnchorPoint(Vec2::ZERO);
	_tiledMap->setPosition(Vec2::ZERO);

	_gridMap = GridMap::create(_tiledMap);

	this->addChild(_tiledMap);

	return true;
}