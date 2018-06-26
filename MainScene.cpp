#include "MainScene.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "Building.h"
#include "string.h"
#include <vector>


using namespace ui;
using namespace CocosDenshion;

USING_NS_CC;

//按钮信息储存
Vector <Sprite*> button;
//建造进度条信息储存
Vector<LoadingBar*>timeBar;
int _curGold(200),_curElectricity(20),goldProducer(0);

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

	//启用定时器回调更新函数
	scheduleUpdate();
	schedule(schedule_selector(MainScene::updateRes), 1.0f);

	//动画初始化
	MainScene::animationInit();

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

	//侧边栏
	auto sidebar1 = Sprite::create("_sidebar.png");
	sidebar1->setScale(0.3);
	sidebar1->setPosition(_screenWidth*0.9 + 72, _screenHeight*0.7);
	sidebar1->setAnchorPoint(Vec2(0, 1));
	addChild(sidebar1);
	auto sidebar2 = Sprite::create("_sidebar.png");
	sidebar2->setScale(0.3);
	sidebar2->setPosition(_screenWidth*0.9 + 72, _screenHeight*0.3);
	sidebar2->setAnchorPoint(Vec2(0, 1));
	addChild(sidebar2);
	auto sidebar = Sprite::create("sidebar1.jpg");
	sidebar->setScale(1);
	sidebar->setPosition(_screenWidth*0.82 + 72, _screenHeight*0.85);
	sidebar->setAnchorPoint(Vec2(0, 1));
	addChild(sidebar);

	
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

	//建筑信息
	std::string name[4] = { "barracks","warfactory","storage","producer" };
	std::vector<std::string> buildingName(name, name + 4);
	std::string fileName[4] = { "barracks.png","warfactory.png","storage.png","producer.png" };
	std::vector<std::string> buildingFileName(fileName, fileName + 4);
	int golds[4] = { -40,-60,-20,-20 };
	std::vector<int> buildingGold(golds, golds + 4);
	int electric[4] = { -5,-8,30,-3 };
	std::vector<int> buildingElectric(electric, electric + 4);
	int time[4] = { 100,100,30,30 };
	std::vector<int> buildingTime(time, time + 4);


	auto barracksButton = Sprite::create("barracks.png");
	barracksButton->setTag(0);
	barracksButton->setPosition(Vec2(35,180));
	sidebar->addChild(barracksButton,1);
	barracksButton->setCascadeOpacityEnabled(true);
	Vec2 barracks_position = barracksButton->getPosition();
	auto barracksButton1 = Sprite::create("barracks.png");
	barracksButton1->setPosition(barracks_position);
	button.pushBack(barracksButton1);
	sidebar->addChild(barracksButton1);
	barracksButton1->setCascadeOpacityEnabled(true);

	auto warfactoryButton = Sprite::create("warfactory.png");
	warfactoryButton->setTag(1);
	warfactoryButton->setScale(0.4);
	warfactoryButton->setPosition(Vec2(35, 130));
	sidebar->addChild(warfactoryButton);
	warfactoryButton->setCascadeOpacityEnabled(true);
	Vec2 warfactory_position = warfactoryButton->getPosition();
	auto warfactoryButton1 = Sprite::create("warfactory.png");
	warfactoryButton1->setPosition(warfactory_position);
	warfactoryButton1->setScale(0.4);
	button.pushBack(warfactoryButton1);
	sidebar->addChild(warfactoryButton1);
	warfactoryButton1->setCascadeOpacityEnabled(true);

	auto storageButton = Sprite::create("storage.png");
	storageButton->setTag(2);
	storageButton->setPosition(Vec2(35,80));
	sidebar->addChild(storageButton);
	storageButton->setCascadeOpacityEnabled(true);
	Vec2 storage_position = storageButton->getPosition();
	auto storageButton1 = Sprite::create("storage.png");
	storageButton1->setPosition(storage_position);
	button.pushBack(storageButton1);
	sidebar->addChild(storageButton1);
	storageButton1->setCascadeOpacityEnabled(true);

	auto producerButton = Sprite::create("producer.png");
	producerButton->setTag(3);
	producerButton->setScale(0.4);
	producerButton->setPosition(Vec2(35,30));
	sidebar->addChild(producerButton);
	producerButton->setCascadeOpacityEnabled(true);
	Vec2 producer_position = producerButton->getPosition();
	auto producerButton1 = Sprite::create("producer.png");
	producerButton1->setPosition(producer_position);
	producerButton1->setScale(0.4);
	button.pushBack(producerButton1);
	sidebar->addChild(producerButton1);
	producerButton1->setCascadeOpacityEnabled(true);

	//创建单点触摸监听器
	auto buildingButtonListener = EventListenerTouchOneByOne::create();
	buildingButtonListener->setSwallowTouches(true);
	buildingButtonListener->onTouchBegan = [=](Touch* touch, Event* event)
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
	buildingButtonListener->onTouchMoved=[](Touch* touch, Event* event)
	{
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		//移动触摸的精灵
		target->setPosition(target->getPosition() + touch->getDelta());

	};
	buildingButtonListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		auto tag = target->getTag();
		target->setOpacity(255);
		target->setPosition(button.at(tag)->getPosition());
		Building* building=nullptr;
		if (_curGold + buildingGold[tag] >= 0 && _curElectricity + buildingElectric[tag] >= 0)
		{
			building = Building::create(buildingFileName[tag]);
			building->setGold(buildingGold[tag]);
			building->setName(buildingName[tag]);
			building->setTime(buildingTime[tag]);
			building->setElectricity(buildingElectric[tag]);
		}
		if (building == nullptr)
		{
			return;
		}
		building->setScale(0.3);
		//创建进度条
		auto bar = LoadingBar::create("CreateBar.png");
		building->_bar = bar;
		
		//设置从左向右递增
		building->_bar->setDirection(LoadingBar::Direction::LEFT);
		//设置位置
		building->_bar->setPercent(0);
		building->_bar->setScale(0.7);
		building->_bar->setScaleX(0.4);
		_tiledMap->addChild(building->_bar, 100);
		building->addToMap(_gridMap, _tiledMap);
		building->_unitManager = _unitManager;
		_curGold += building ->getGold();
		if (building->getName() == "producer")
		{
			goldProducer++;
		}
		_curElectricity += building->getElectricity();
		building->setPosition(_tiledMap->convertToNodeSpace(touch->getLocation()));
		if (building->getName() !="warfactory")
		{
			building->_bar->setPosition(Vec2(building->getPosition().x, building->getPosition().y + 20));
		}
		else
		{
			building->_bar->setPosition(Vec2(building->getPosition().x, building->getPosition().y + 25));
		}
		building->schedule(schedule_selector(Building::update),1.0f);
		building->setOpacity(0);
		building->runAction(FadeIn::create(building->getTime()));
		auto creation = AnimationCache::getInstance()->getAnimation("create");
		auto animate = Animate::create(creation);
		auto repeat = Repeat::create(animate,building->getTime()/3+1);
		auto animationSprite = Sprite::createWithSpriteFrameName("animate1.png");
		_tiledMap->addChild(animationSprite,100);
		animationSprite->setPosition(building->getPosition());
		//animationSprite->runAction(repeat);
		Sequence* seq = Sequence::create(repeat, RemoveSelf::create(true), NULL);
		animationSprite->runAction(seq);


	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingButtonListener, barracksButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingButtonListener->clone(), warfactoryButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingButtonListener->clone(), storageButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingButtonListener->clone(), producerButton);



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

		case EventKeyboard::KeyCode::KEY_H:
			if (sidebar1->getPosition().x>_screenWidth)
			{
				auto moveBy = MoveBy::create(1, Point(-83, 0));
				sidebar->runAction(moveBy);
				sidebar1->runAction(moveBy->clone());
				sidebar2->runAction(moveBy->clone());
			}
			else
			{
				auto moveBy = MoveBy::create(1, Point(83, 0));
				sidebar->runAction(moveBy);
				sidebar1->runAction(moveBy->clone());
				sidebar2->runAction(moveBy->clone());
			}
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
	_count++;
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
	_curGold += goldProducer;
	_goldLabel->setString(StringUtils::format("%d", _curGold));
	_electricityLabel->setString(StringUtils::format("%d", _curElectricity));
}

void MainScene::animationInit()
{
	//创建精灵帧缓存单例对象并添加纹理到缓存中
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animation.plist");
	auto animation = Animation::create();
	for (int i = 1; i < 15; i++)
	{
		std::string name = StringUtils::format("animate%d.png", i);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
	}
	animation->setDelayPerUnit(3.0f / 14.0f);
	animation->setRestoreOriginalFrame(true);
	AnimationCache::getInstance()->addAnimation(animation, "create");



}