#include "RoomScene.h"
#include "Classes/MainScene.h"

bool RoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	//初始化服务端
	_server = LocalServer::create();
	this->addChild(_server);
	Sleep(3000);//睡一会

	//等待服务端初始化后初始化客户端
	_client = Client::create();
	this->addChild(_client);
	//Sleep(2000);

	//初始化游戏人数标签标签和开始游戏按钮
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto width = visibleSize.width;
	auto height = visibleSize.height;
	log("%f,%f", width, height);

	_clientNum = Label::create();
	this->addChild(_clientNum, 5);
	_clientNum->setPosition(width / 2, height / 2 + 20);
	_clientNum->setString("fuck!");

	_readyClientNum = Label::create();
	this->addChild(_readyClientNum, 5);
	_readyClientNum->setPosition(width / 2, height / 2);
	_readyClientNum->setString("fuck!");

	
	auto sequence = Sequence::create(
		DelayTime::create(3.0f),//延时确保同时开始
		CallFunc::create([=] {
		_server->startGame();
	}),
		NULL
		);
	//this->runAction(sequence);
	//切换场景
	auto button = Button::create("picture/MenuItems/ButtonStartGame.png", "picture/MenuItems/ButtonStartGameSelected.png");
	button->addTouchEventListener([=](Ref*,
		Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			
			auto transiton = TransitionSlideInL::create(2.f, MainScene::create(_client, _server));
			Director::getInstance()->pushScene(transiton);
			_server->startGame();
		}
	});
	this->addChild(button);
	button->setPosition(Vec2(width / 2, height / 2 - 40));
	
	scheduleUpdate();//更新人数

	return true;
}

void RoomScene::update(float delta)
{
	_clientNum->setString(string("All Clients num: ").append(num2Str(_server->getClientNum())));
	_readyClientNum->setString(string("Ready Clients num: ").append(num2Str(_server->getReadyClientNum())));
}

const string RoomScene::num2Str(int num)
{
	char buff[10] = "";
	sprintf(buff, "%d", num);

	return string(buff);
}
