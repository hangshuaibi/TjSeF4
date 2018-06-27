#include "RoomScene.h"
#include "Classes/MainScene.h"

bool RoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	ScenePool::preScene = this;

	auto background = Sprite::create("scene/room.jpg");
	this->addChild(background);
	background->setAnchorPoint(cocos2d::Vec2::ZERO);

	//初始化服务端
	_server = LocalServer::create();
	this->addChild(_server);
	Sleep(300);//睡一会

	//等待服务端初始化后初始化客户端
	_client = Client::create("127.0.0.1");
	this->addChild(_client);
	Sleep(200);

	_client->sendMessage("Client is ready");

	//初始化游戏人数标签标签和开始游戏按钮
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto width = visibleSize.width;
	auto height = visibleSize.height;
	log("%f,%f", width, height);

	_clientNum = Label::create();
	this->addChild(_clientNum, 5);
	_clientNum->setPosition(width / 2, height / 2 + 20);
	//_clientNum->setString("f**k!");

	_readyClientNum = Label::create();
	this->addChild(_readyClientNum, 5);
	_readyClientNum->setPosition(width / 2, height / 2);
	//_readyClientNum->setString("f**k!");

	
	
	auto button = Button::create("scene/start.png");
	button->addTouchEventListener([=](Ref*,
		Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			_server->startGame();
		}
	});
	this->addChild(button);
	button->setPosition(Vec2(width / 2, height / 2 - 40));
	button->setTouchEnabled(true);

	scheduleUpdate();//更新人数

	return true;
}

void RoomScene::update(float delta)
{
	if (_client->isStart())
	{
		auto transiton = TransitionSlideInL::create(2.f, MainScene::create(_client));
		Director::getInstance()->pushScene(transiton);
	}
	_clientNum->setString(string("All Clients num: ").append(num2Str(_server->getClientNum())));
	_readyClientNum->setString(string("Ready Clients num: ").append(num2Str(_server->getReadyClientNum())));
}

const string RoomScene::num2Str(int num)
{
	char buff[10] = "";
	sprintf(buff, "%d", num);

	return string(buff);
}
