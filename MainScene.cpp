#include "MainScene.h"
#include "cocos2d.h"
#include "GameManager.h"
#include"SimpleAudioEngine.h"
#include"ui/CocosGUI.h"
#include"Building.h"
using namespace ui;
using namespace CocosDenshion;

USING_NS_CC;

//按钮信息储存
Vector <Sprite*> button;
int _curGold(200), _curElectricity(20),_goldProducer(0);

#define PATHFINDER_TEST 0

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
	//控制面板
	_controlPanel = ControlPanel::create();
	this->addChild(_controlPanel, 10);
	_controlPanel->setPosition(_screenWidth, _screenHeight);


	int _barracksGold = -40, _barracksElectricity = -5,
		_warfactoryGold = -60, _warfactoryElectricity = -8,
		_storageGold = -20, _storageElectricity = 30,
		_producerGold = -20, _producerElectricity = -3;

	//启用定时器回调更新函数
	scheduleUpdate();
	schedule(schedule_selector(MainScene::updateRes), 1.0f);

	//获取GameManager实例
	_gameManager = GameManager::create(this);
	this->addChild(_gameManager);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	//暂时用lostTemple.tmx
	_tiledMap = TMXTiledMap::create("Map/LostTemple.tmx");

	_tiledMap->setAnchorPoint(Vec2::ZERO);
	_tiledMap->setPosition(Vec2::ZERO);

	_gridMap = GridMap::create(_tiledMap);

	this->addChild(_gridMap);

	this->addChild(_tiledMap, 0);

	_unitManager = UnitManager::create();
	_unitManager->_gridMap = _gridMap;
	_unitManager->_tiledMap = _tiledMap;
	this->addChild(_unitManager);

	_mouseRect = MouseRect::create();
	_mouseRect->setVisible(false);
	_tiledMap->addChild(_mouseRect, 10);

	//金钱图标
	auto gold = Sprite::create("money.jpg");
	gold->setPosition(50, _screenHeight*0.9);
	gold->setScale(0.05);
	this->addChild(gold);
	_goldLabel = Label::createWithSystemFont("200", "Arial", 32);
	_goldLabel->setPosition(100, _screenHeight*0.9);
	this->addChild(_goldLabel);

	//电力图标
	auto electricity = Sprite::create("electricity.png");
	electricity->setPosition(150, _screenHeight*0.9);
	this->addChild(electricity);
	_electricityLabel = Label::createWithSystemFont("20", "Arial", 32);
	_electricityLabel->setPosition(200, _screenHeight*0.9);
	this->addChild(_electricityLabel);

	auto barracksButton = Sprite::create("barracks.png");
	barracksButton->setScale(0.3);
	barracksButton->setTag(0);
	barracksButton->setPosition(Vec2(_screenWidth - 40, _screenHeight - 80));
	this->addChild(barracksButton);
	Vec2 barracks_position = barracksButton->getPosition();
	auto barracksButton1 = Sprite::create("barracks.png");
	barracksButton1->setPosition(barracks_position);
	barracksButton1->setScale(0.3);
	button.pushBack(barracksButton1);
	this->addChild(barracksButton1);

	auto warfactoryButton = Sprite::create("warfactory.png");
	warfactoryButton->setTag(1);
	warfactoryButton->setScale(0.3);
	warfactoryButton->setPosition(Vec2(_screenWidth - 40, _screenHeight - 130));
	this->addChild(warfactoryButton);
	Vec2 warfactory_position = warfactoryButton->getPosition();
	auto warfactoryButton1 = Sprite::create("warfactory.png");
	warfactoryButton1->setPosition(warfactory_position);
	warfactoryButton1->setScale(0.3);
	button.pushBack(warfactoryButton1);
	this->addChild(warfactoryButton1);

	auto storageButton = Sprite::create("storage.png");
	storageButton->setTag(2);
	storageButton->setScale(0.3);
	storageButton->setPosition(Vec2(_screenWidth - 40, _screenHeight - 160));
	this->addChild(storageButton);
	Vec2 storage_position = storageButton->getPosition();
	auto storageButton1 = Sprite::create("storage.png");
	storageButton1->setPosition(storage_position);
	storageButton1->setScale(0.3);
	button.pushBack(storageButton1);
	this->addChild(storageButton1);

	auto producerButton = Sprite::create("producer.png");
	producerButton->setTag(3);
	producerButton->setScale(0.25);
	producerButton->setPosition(Vec2(_screenWidth - 40, _screenHeight - 200));
	this->addChild(producerButton);
	Vec2 producer_position = producerButton->getPosition();
	auto producerButton1 = Sprite::create("producer.png");
	producerButton1->setPosition(producer_position);
	producerButton1->setScale(0.3);
	button.pushBack(producerButton1);
	this->addChild(producerButton1);

	//创建单点触摸监听器
	auto _listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		//获得当前触摸事件的目标对象
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		//获得当前的触摸点
		Point locationPoint = target->convertToNodeSpace(touch->getLocation());
		//获得触摸坐标
		Vec2 touchLocation = touch->getLocation();
		

		//获得触摸对象的contentSize
		Size s = target->getContentSize();
		//获得位置矩形
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationPoint))
		{
			target->setOpacity(180);
			return true;
		}
		return false;
	};
	_listener->onTouchMoved=[](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		//移动触摸的精灵
		target->setPosition(target->getPosition() + touch->getDelta());

	};
	_listener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto tag = target->getTag();
		target->setOpacity(255);

		target->setPosition(button.at(tag)->getPosition());
		Building* building=nullptr;
		switch (tag)
		{
		case 0:
			if (_curgold + _barracksGold < 0 || _curElectricity + _barracksElectricity < 0)
			{
				break;
			}
			building= (Barracks*)Barracks::create();
			break;
		case 1: 
			if (_curgold + _warfactoryGold < 0 || _curElectricity + _warfactoryElectricity < 0)
			{
				break;
			}
			building = (Warfactory*)Warfactory::create();
			break;
		case 2:  
			if (_curgold + _storageGold < 0 || _curElectricity + _storageElectricity < 0)
			{
				break;
			}
			building = (Storage*)Storage::create();
			break;
		case 3:  
			if (_curgold + _producerGold < 0 || _curElectricity + _producerElectricity < 0)
			{
				break;
			}
			building = (Producer*)Producer::create();
			break;
		}
		if (building == nullptr)
		{
			return;
		}
		building->setScale(0.3);
		building->addToMap(_gridMap, _tiledMap);
		building->_unitManager = _unitManager;
		_curgold += building ->getGold();
		_goldProducer += building->isgold();
		_curElectricity += building->getElectricity();
		building->setPosition(_tiledMap->convertToNodeSpace(touch->getLocation()));
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, barracksButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener->clone(), warfactoryButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener->clone(), storageButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener->clone(), producerButton);



	Unit* sprite = Unit::create("airplane_0.png");

	//sprite->setScale(0.1);
	sprite->addToMap(_gridMap, _tiledMap);
	sprite->_unitManager = _unitManager;
	_unitManager->_getUnitById.insert(std::make_pair(-1, sprite));
	//_unitManager->_selectId.push_back(-1);

	log("screenWidth: %d, screenHeight: %d\n", _screenWidth, _screenHeight);
	log("map: %d, %d", _tiledMap->getPosition().x, _tiledMap->getPosition().y);

	sprite->setProperties();

	sprite->setPosition(_gridMap->getPoint(Grid(8, 6)));

	//sprite->setDestination(Grid(18, 120));
	//sprite->findPath();
	sprite->setState(Unit::State::MOVING);
	sprite->schedule(schedule_selector(Unit::update));

	auto testMouseListener = EventListenerTouchOneByOne::create();

	testMouseListener->onTouchBegan = [=](Touch* touch, Event* /**/) {
		sprite->setState(Unit::WONDERING);

		return true;
	};

	testMouseListener->onTouchEnded = [=](Touch* touch, Event* /**/) {
		Point touchL = touch->getLocation();

		auto dest = touchL - _tiledMap->getPosition();
		auto gridDest = _gridMap->getGrid(dest);
		auto validDest = _gridMap->findValidGridNear(gridDest);

		sprite->setDestination(validDest);

		sprite->findPath();
		sprite->setState(Unit::MOVING);
	};


	//test rectSelect
	_unitManager->createUnit(0);

	auto mouseListener = EventListenerTouchOneByOne::create();
	mouseListener->setSwallowTouches(true);//
	mouseListener->onTouchBegan = [=](Touch* touch, Event* /*event*/) {
		//记录鼠标选框的第一个点
		_mouseRect->_touchStart = touch->getLocation();
		_mouseRect->start = _tiledMap->convertToNodeSpace(_mouseRect->_touchStart);

		return true;
	};
	mouseListener->onTouchMoved = [=](Touch* touch, Event* /*event*/) {
		//鼠标移动时更新选框终点
		_mouseRect->_touchEnd = touch->getLocation();

		//把Began的语句移到这里，start就变成（0，0）？？？？？
		//_mouseRect->start = _tiledMap->convertToNodeSpace(_mouseRect->_touchStart);
		_mouseRect->end = _tiledMap->convertToNodeSpace(_mouseRect->_touchEnd);

		_mouseRect->clear();

		_mouseRect->setVisible(true);
		_mouseRect->drawRect(_mouseRect->start, _mouseRect->end, Color4F(0, 1, 0, 1));

	};
	mouseListener->onTouchEnded = [=](Touch* touch, Event* /*event*/) {
		_mouseRect->clear();
	
		_mouseRect->_touchEnd = touch->getLocation();
		_mouseRect->end = _tiledMap->convertToNodeSpace(_mouseRect->_touchEnd);

		//select id in the rect
		_unitManager->selectUnitByRect(Rect{ MIN(_mouseRect->start.x, _mouseRect->end.x),
			MIN(_mouseRect->start.y, _mouseRect->end.y),
			abs(_mouseRect->start.x - _mouseRect->end.x),
			abs(_mouseRect->start.y - _mouseRect->end.y)
		});

		_mouseRect->setVisible(false);
	};

	if (!PATHFINDER_TEST)
	{
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
	}
	else {
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(testMouseListener, this);
	}


	auto keyboardListener = EventListenerKeyboard::create();

	//设置按键响应函数
	keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
	{
		//log("Key with keycod % d pressed");

		return true;
	};

	keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
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
			_gameManager->focusOnBase();
			break;

		default:
			break;
		}

	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	//设置鼠标监听器
	auto mouse_event = EventListenerMouse::create();
	mouse_event->onMouseMove = [=](Event *event)
	{
		EventMouse* e = static_cast<EventMouse*>(event);
		//获取当前鼠标坐标

		_cursorPosition = Vec2(e->getCursorX(), e->getCursorY());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse_event, this);
	
	return true;
}

void MainScene::update(float delta)
{

	_gameManager->scrollMap();
}

bool ControlPanel::init()
{
	if (!Menu::init())
	{
		return false;
	}

	_fighter = MenuItemImage::create("fighter.png",
		"fighter.png", CC_CALLBACK_1(ControlPanel::createFighterCallBack, this));
	assert(_fighter != nullptr);

	_fighter->setAnchorPoint(Vec2(1, 1));
	_fighter->setPosition(getContentSize().width, getContentSize().height);
	//把坦克的图片挂在控制面板上
	this->addChild(_fighter, 10);



	return true;
}

void ControlPanel::createFighterCallBack(Ref* psender)
{
	_fighter->setOpacity(100);
}

void MainScene::updateRes(float delta)
{
	_curgold +=5* _goldProducer;
	_goldLabel->setString(StringUtils::format("%d", _curgold));
	_electricityLabel->setString(StringUtils::format("%d", _curElectricity));
}
