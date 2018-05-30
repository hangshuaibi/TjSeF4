#include "MainScene.h"
#include "cocos2d.h"
#include "GameManager.h"
USING_NS_CC;

int MainScene::_screenWidth = 0, MainScene::_screenHeight = 0;  //屏幕宽度和高度
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
	//¸¸Ààº¯Êý
	if (!Scene::init())
	{
		return false;
	}

	//启用定时器回调更新函数
	scheduleUpdate();

	//获取GameManager实例
	auto gameManager = GameManager::create(this);
	addChild(gameManager);

	//获得设备可见视图大小
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//获取屏幕宽度和高度
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	//ÔÝÊ±ÓÃlostTemple.tmx
	_tiledMap = TMXTiledMap::create("Map/LostTemple.tmx");

	_tiledMap->setAnchorPoint(Vec2::ZERO);
	_tiledMap->setPosition(Vec2::ZERO);

	_gridMap = GridMap::create(_tiledMap);
	//?
	this->addChild(_gridMap);

	this->addChild(_tiledMap, 0);

	_unitManager = UnitManager::create();
	_unitManager->_gridMap = _gridMap;
	this->addChild(_unitManager);

	_mouseRect = MouseRect::create();
	_mouseRect->setVisible(false);
	_tiledMap->addChild(_mouseRect, 10);

	auto mouseListener = EventListenerTouchOneByOne::create();
	mouseListener->setSwallowTouches(true);//
	mouseListener->onTouchBegan = [=](Touch* touch, Event* /*event*/) {
		_mouseRect->_touchStart = touch->getLocation();

		return true;
	};
	mouseListener->onTouchMoved = [=](Touch* touch, Event* /*event*/) {
		_mouseRect->_touchEnd = touch->getLocation();

		_mouseRect->start = _tiledMap->convertToNodeSpace(_mouseRect->_touchStart);
		_mouseRect->end = _tiledMap->convertToNodeSpace(_mouseRect->_touchEnd);

		_mouseRect->clear();

		_mouseRect->setVisible(true);
		_mouseRect->drawRect(_mouseRect->start, _mouseRect->end, Color4F(0, 1, 0, 1));

	};

	mouseListener->onTouchEnded = [=](Touch* touch, Event* /*event*/) {
		_mouseRect->clear();

		//select id in the rect
		_unitManager->selectUnitByRect(Rect{ MIN(_mouseRect->start.x, _mouseRect->end.x),
			MIN(_mouseRect->start.y, _mouseRect->end.y),
			abs(_mouseRect->start.x - _mouseRect->end.x),
			abs(_mouseRect->start.y - _mouseRect->end.y)
			});

		_mouseRect->setVisible(false);
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);


	auto keyboardListener = EventListenerKeyboard::create();

	//设置按键响应函数
	keyboardListener->onKeyPressed = [gameManager](EventKeyboard::KeyCode keyCode, Event* event)
	{
		//log("Key with keycod % d pressed");

		return true;
	};

	keyboardListener->onKeyReleased = [gameManager, this](EventKeyboard::KeyCode keyCode, Event* event)
	{
		auto mapCenter = this->_tiledMap->getPosition();
		Director::getInstance()->getVisibleSize();
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_W:
			mapCenter += Vec2(0, -50);
			if (this->_tiledMap->getBoundingBox().containsPoint(Vec2(0, 50) + Director::getInstance()->getVisibleSize()))
				this->_tiledMap->setPosition(mapCenter);
			break;
		case EventKeyboard::KeyCode::KEY_A:
			mapCenter += Vec2(50, 0);
			if (this->_tiledMap->getBoundingBox().containsPoint(Vec2(-50, 0)))
				this->_tiledMap->setPosition(mapCenter);
			break;
		case EventKeyboard::KeyCode::KEY_S:
			mapCenter += Vec2(0, 50);
			if (this->_tiledMap->getBoundingBox().containsPoint(Vec2(0, -50)))
				this->_tiledMap->setPosition(mapCenter);
			break;
		case EventKeyboard::KeyCode::KEY_D:
			mapCenter += Vec2(-50, 0);
			if (this->_tiledMap->getBoundingBox().containsPoint(Vec2(50, 0) + Director::getInstance()->getVisibleSize()))
				this->_tiledMap->setPosition(mapCenter);
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			gameManager->focusOnBase();
			break;
		default:
			break;
		}

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);


	
	//设置鼠标监听器
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [this, gameManager](Event *event)
	{
		EventMouse* e = static_cast<EventMouse*>(event);

	//获取当前鼠标坐标
		_crusorPosition = Vec2(e->getCursorX(), e->getCursorY());
	
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse_event, this);
}

void MainScene::update(float f)
{
	auto gameManager = GameManager::create(this);
	addChild(gameManager);
	gameManager->scrollMap();
	
}