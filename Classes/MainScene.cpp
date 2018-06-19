#include "MainScene.h"
#include "cocos2d.h"
#include "GameManager.h"
#include"SimpleAudioEngine.h"
#include"ui/CocosGUI.h"
#include "RealUnit.h"
#include "Button.h"


using namespace ui;
using namespace CocosDenshion;

#include <string>

USING_NS_CC;
typedef Unit::Type Type;

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
	//客户端
	_client = Client::create();
	this->addChild(_client);

	//挂起当前线程，等待客户端初始化
	Sleep(2000);
	//_client->sendMessage("Client is ready");
	//_client->sendMessage(_client->ip);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	//启用定时器回调更新函数
	scheduleUpdate();

	//消息提示
	_notice = Label::create();
	_notice->setPosition(_screenWidth / 2, _screenHeight - 10.0f);
	_notice->setString("so boring");
	this->addChild(_notice, 10);

	//获取GameManager实例
	_gameManager = GameManager::create(this);
	this->addChild(_gameManager);

	//暂时用lostTemple.tmx
	_tiledMap = TMXTiledMap::create("Map/LostTemple.tmx");

	_tiledMap->setAnchorPoint(Vec2::ZERO);
	_tiledMap->setPosition(Vec2::ZERO);
	this->addChild(_tiledMap, 0);

	//控制面板
	_controlPanel = ControlPanel::create();
	_tiledMap->addChild(_controlPanel, 10);
	_controlPanel->setPosition(0, 0);
	//_controlPanel->setAnchorPoint(Vec2(0.f, 0.f));
	_controlPanel->setVisible(false);

	_gridMap = GridMap::create(_tiledMap);
	this->addChild(_gridMap);

	initLabel();

	//------------//
	_displayValueLabel[0] = Text::create("Press enter to start to chat!", "Marker Felt.ttf", 10);
	_displayValueLabel[0]->setAnchorPoint(Vec2(0, 0));
	_displayValueLabel[0]->setPosition(Vec2(20, visibleSize.height - 40));

	this->addChild(_displayValueLabel[0]);

	_displayValueLabel[1] = Text::create(" ", "Marker Felt.ttf", 10);
	_displayValueLabel[1]->setAnchorPoint(Vec2(0, 0));
	_displayValueLabel[1]->setPosition(Vec2(20, visibleSize.height - 60));

	this->addChild(_displayValueLabel[1]);
	//-------//

	_unitManager = UnitManager::createWithScene(this);
	_unitManager->schedule(schedule_selector(UnitManager::update));
	
	this->addChild(_unitManager);
	
	_mouseRect = MouseRect::create();
	_mouseRect->setVisible(false);
	_tiledMap->addChild(_mouseRect, 10);
	//-----------------------//
	//创建单点触摸监听器
	auto buildingListener = EventListenerTouchOneByOne::create();
	buildingListener->setSwallowTouches(true);

	buildingListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<BButton*>(event->getCurrentTarget());
		assert(target != nullptr);

		target->onPress();
		return target->isTouched(touch);
	};
	buildingListener->onTouchMoved=[](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<BButton*>(event->getCurrentTarget());
		assert(target != nullptr);
		auto copy = target->copy();

		//移动触摸的精灵
		copy->setPosition(copy->getPosition() + touch->getDelta());

	};
	buildingListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		auto target = dynamic_cast<BButton*>(event->getCurrentTarget());
		assert(target != nullptr);

		target->onRelease();
	};
	//-----------------------//
	initButton(buildingListener);


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

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);
	

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

		if (_isInput)
		{
			return;
		}
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
		case EventKeyboard::KeyCode::KEY_ENTER:
			_inputBar->setVisible(true);
			_chatWindow->setTouchAreaEnabled(true);
			_isInput = true;
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
	
	//-------------------//
	_chatWindow = cocos2d::ui::TextField::create("  ", "Arial", 18);
	_chatWindow->setMaxLengthEnabled(true);
	_chatWindow->setMaxLength(20);
	_chatWindow->setTouchSize(Size(200, 60));
	_chatWindow->setFontSize(7);
	_chatWindow->setTouchAreaEnabled(true);
	_chatWindow->setPosition(Point(visibleSize.width / 3 * 1.2,
		(visibleSize.height - 90) / 6 * 1));
	_chatWindow->addEventListener(CC_CALLBACK_2(MainScene::textFieldEvent, this));

	this->addChild(_chatWindow, 2);

	//int playerId = _unitManager->_playerId;

	_sendMessageButton = Button::create("button.png");
	this->addChild(_sendMessageButton);
	_sendMessageButton->setPosition(Vec2(
		visibleSize.width / 2 * 1.7,
		(visibleSize.height - 90) / 6 * 1));
	_sendMessageButton->setTitleText("send message");
	_sendMessageButton->setTitleFontSize(7);
	_sendMessageButton->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {

		if (type == Widget::TouchEventType::ENDED) {
			auto chatMessage = _chatWindow->getStringValue();

			auto message = _gameManager->gameEncodeChat("g", _unitManager->_playerId, chatMessage);  
			_client->sendMessage(message);


			_chatWindow->setString("");
			_inputBar->setVisible(false);
			_chatWindow->setTouchAreaEnabled(false);

			_isInput = false;
		}

	});

	_inputBar = Sprite::create("InputBar.png");
	_inputBar->setPosition(Point(visibleSize.width / 3 * 1.2,
		(visibleSize.height - 90) / 6 * 1));
	_inputBar->setVisible(false);

	this->addChild(_inputBar, 1);
	//------------------//

	return true;
}

void MainScene::textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type)
{
	if(cocos2d::ui::TextField::EventType::ATTACH_WITH_IME==type)
	{
		_isInput = true;
		//cocos2d::ui::TextField* textField = dynamic_cast<cocos2d::ui::TextField*>(pSender);
		//Size screenSize = CCDirector::getInstance()->getWinSize();
	}
	
}

void MainScene::update(float delta)
{
	_gameManager->scrollMap();
}

void MainScene::initButton(EventListenerTouchOneByOne* buildingListener)
{//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	std::vector<BButton*> buttons = {
		BButton::create(this, Type::FACTORY, "picture/units/factory_0.png"),
		BButton::create(this, Type::SOLDIERCAMP, "picture/units/soldiercamp_0.png"),
		BButton::create(this, Type::MINE, "picture/units/barracks.png"),
		BButton::create(this, Type::ELECTRICITYFACTORY, "picture/units/storage.png"),
	};
	std::vector<Point> points = { Point(_screenWidth - 40, _screenHeight - 80),
		Point(_screenWidth - 40, _screenHeight - 130),Point(_screenWidth - 40, _screenHeight - 180),
		Point(_screenWidth - 40, _screenHeight - 230),
	};

	for (int i = 0;i < 4;++i)
	{
		this->addChild(buttons[i]);
		buttons[i]->setPosition(points[i]);
		auto listener = i == 0 ? (buildingListener) : (buildingListener->clone());
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, buttons[i]);
	}

	/*auto factoryButton = BButton::create(this, Type::FACTORY, "picture/units/factory_0.png");
	auto campButton = BButton::create(this, Type::SOLDIERCAMP, "picture/units/soldiercamp_0.png");
	auto mineButton = BButton::create(this, Type::MINE, "picture/units/barracks.png");
	auto eleFactoryButton = BButton::create(this, Type::ELECTRICITYFACTORY, "picture/units/storage.png");

	factoryButton->setPosition(_screenWidth - 40, _screenHeight - 80);
	campButton->setPosition(_screenWidth - 40, _screenHeight - 130);
	mineButton->setPosition(_screenWidth - 40, _screenHeight - 180);
	eleFactoryButton->setPosition(_screenWidth - 40, _screenHeight - 230);

	this->addChild(factoryButton, 10);
	this->addChild(campButton, 10);
	this->addChild(mineButton, 10);
	this->addChild(eleFactoryButton, 10);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingListener, factoryButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingListener->clone(), campButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingListener->clone(), mineButton);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(buildingListener->clone(), eleFactoryButton);*/
}

void MainScene::initLabel()
{
	auto goldPng = Sprite::create("picture/ui/gold_.png");
	auto powerPng = Sprite::create("picture/ui/power.png");
	goldPng->setPosition(15.0f, _screenHeight - 15.0f);
	powerPng->setPosition(_screenWidth / 4 - 10.0f, _screenHeight - 15.0f);
	powerPng->setScale(0.7f);

	this->addChild(goldPng);
	this->addChild(powerPng);

	_goldLabel = Label::create();
	_powerLabel = Label::create();

	_goldLabel->setPosition(goldPng->getPosition().x + goldPng->getContentSize().width * 0.7f,
		goldPng->getPosition().y);
	_powerLabel->setPosition(powerPng->getPosition().x + powerPng->getContentSize().width * 0.7f,
		powerPng->getPosition().y);

	this->addChild(_goldLabel);
	this->addChild(_powerLabel);

	_goldLabel->setColor(Color3B::YELLOW);
	_powerLabel->setColor(Color3B::YELLOW);
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
	_tank = MenuItemImage::create("tank.png",
		"tank.png", CC_CALLBACK_1(ControlPanel::createTankCallBack, this));
	assert(_tank != nullptr);

	_fighter->setScale(0.8f);
	_tank->setScale(0.8f);

	_fighter->setAnchorPoint(Vec2::ZERO);
	_fighter->setPosition(getContentSize().width / 2 - 10, getContentSize().height / 2);
	_tank->setAnchorPoint(Vec2::ZERO);
	_tank->setPosition(getContentSize().width / 2 + 10, getContentSize().height / 2);

	//把图片挂在控制面板上
	this->addChild(_fighter, 10);
	this->addChild(_tank, 10);

	return true;
}

class Factory;
void ControlPanel::createFighterCallBack(Ref* psender)
{
	//_fighter->setOpacity(100);
	setVisible(false);
	_factory->localCreateUnit_(Type::FIGHTER);
}

void ControlPanel::createTankCallBack(Ref* pSender)
{
	//_tank->setOpacity(100);
	setVisible(false);
	_factory->localCreateUnit_(Type::TANK);
}
