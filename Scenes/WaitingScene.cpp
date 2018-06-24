#include "WaitingScene.h"
#include <string>
#include "Network/Client.h"
#include "Classes/MainScene.h"

using std::string;

namespace {//匿名空间
	bool connectSuccess = false;//是否连接成功
	bool connecting = false;//正在连接
	bool canInput = true;//正在连接或者已经连接成功不允许再次输入ip
	int timer = 0;
	Client* client = nullptr;//客户端
	Label* noteLabel = nullptr;//消息提示标签
}

bool WaitingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	ScenePool::preScene = this;

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto background = Sprite::create("scene/waiting.jpg");
	this->addChild(background);
	background->setAnchorPoint(cocos2d::Vec2::ZERO);


	//标签初始化
	noteLabel = Label::create();
	noteLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.9));
	this->addChild(noteLabel);

	//一个输入ip的窗口 + 一个确认连接的按钮
	_inputIpWindow = cocos2d::ui::TextField::create("  ", "Arial", 7);
	_inputIpWindow->setMaxLengthEnabled(true);
	_inputIpWindow->setMaxLength(15);//ipv4地址最长为15
	_inputIpWindow->setTouchSize(Size(200, 60));
	_inputIpWindow->setTouchAreaEnabled(true);
	_inputIpWindow->setPosition(Point(visibleSize.width / 3, visibleSize.height / 2));
	
	//窗口实体
	auto realWindow = Sprite::create("scene/InputBar.png");
	realWindow->setPosition(Point(visibleSize.width / 3, visibleSize.height / 2));

	this->addChild(_inputIpWindow, 2);
	this->addChild(realWindow, 1);

	//回车键（
	auto enterButton = Button::create("button.png");
	this->addChild(enterButton);
	enterButton->setPosition(Point(visibleSize.width * 0.7f, visibleSize.height / 2));
	enterButton->setTitleText("enter");
	enterButton->setTitleFontSize(7);
	enterButton->addTouchEventListener(CC_CALLBACK_2(WaitingScene::EnterHandle, this));

	scheduleUpdate();

	return true;
}//init

void WaitingScene::update(float delta)
{
	if (connecting)
	{
		if (++timer <= 120)
		{
			if (timer < 50)return;//不要猴急
			
			if (client->isConnected())
			{
				connecting = false;
				connectSuccess = true;
				notice(Notice::CONNECT_SUCCESS);
				client->sendMessage("Client is ready");//这里可以插名字
			}
			
		}
		else {
			canInput = true;
			connecting = false;
			connectSuccess = false;
			notice(Notice::CONNET_FAIL);

			this->removeChild(client, true);
			client = nullptr;
		}
	}
	else if(timer != 0){
		//确保每次开始连接timer都是0
		timer = 0;
	}
	
	if (connectSuccess)
	{
		if (client->isStart())
		{
			auto transiton = TransitionSlideInL::create(2.f, MainScene::create(client));
			Director::getInstance()->pushScene(transiton);
		}
	}
}

void WaitingScene::EnterHandle(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED && canInput) {
		string ip = _inputIpWindow->getString();
		_inputIpWindow->setString("");//置空

		if (!isIpValid(ip))
		{
			notice(Notice::VALID_IP);
			return;
		}

		client = Client::create(ip);
		this->addChild(client);


		canInput = false;
		connecting = true;
		notice(Notice::CONNECT_TRY);
	}
}

void WaitingScene::notice(Notice type)
{
	static string infos[10] = {
		"Connecting, be patient.",
		"Wow, Connect Success! Just wait for game start",
		"Emmm, Connect Failed~, try again",
		"Valid ip! try again",
	};
	noteLabel->setString(infos[type]);
}

bool WaitingScene::isIpValid(const string& ip)
{
	for (int size_ = ip.size();size_ > 0;--size_)
	{
		if ((ip[size_ - 1]<'0' || ip[size_ - 1]>'9') && ip[size_ - 1] != '.')
			return false;
	}

	int pointCount = 0;
	for (int beginPos = 0, endPos = ip.find('.');
		endPos!=string::npos;
		beginPos = endPos + 1, endPos = ip.find('.', endPos + 1))
	{
		++pointCount;
		if (!isPartOfIp(ip.substr(beginPos, endPos - beginPos)))
		{
			return false;
		}
	}
	return pointCount == 3;
}

bool WaitingScene::isPartOfIp(const string& str)
{
	int num;
	sscanf(str.c_str(), "%d", &num);

	return 0 <= num && 255 >= num
		&& str.size() < 4;
}