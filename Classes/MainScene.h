#pragma once

/*这是游戏的主场景，赶快抓上小伙伴一起愉快玩耍吧！*/

#include "GridMap.h"
#include "cocos2d.h"
#include "UnitManager.h"
#include "GameManager.h"
#include "Network/Client.h"
//#include "Network/LocalServer.h"
//#include "RealUnit.h"

USING_NS_CC;

namespace ScenePool {
	static cocos2d::Scene* preScene = nullptr;
}

//矩形选框
struct MouseRect:public DrawNode {
	CREATE_FUNC(MouseRect);
	Point _touchStart, _touchEnd;

	Point start, end;//in the _tileMap
	//derive from DrawNode for memfun DrawRect
};

class ControlPanel;
class GameManager;
class UnitManager;
class BButton;
class MainScene :public Scene {
	friend class GameManager;
	friend class ControlPanel;//for create callback function
	friend class UnitManager;
	friend class BButton;

public:
	static MainScene* create(Client* client = nullptr);

	void initNetwork(Client* client);

	static MainScene* createScene();
private:
	bool init();

	void initButton(EventListenerTouchOneByOne* buildingListener);

	void initLabel();
private:
	Label* _goldLabel = nullptr;//金钱的显示
	Label* _powerLabel = nullptr;//电力的显示
	//---------------//
	Text * _displayValueLabel[2] = { nullptr ,nullptr , };         //聊天消息显示栏
	//Text * _displayValueLabel2 = nullptr;
	TextField* _chatWindow = nullptr;
	Sprite* _inputBar = nullptr;
	Button* _sendMessageButton = nullptr;

	bool _isInput = false;
	//-------------//
	ControlPanel* _controlPanel = nullptr;//生产飞机和坦克的控制面板
	Label* _notice = nullptr;//消息提示

	cocos2d::Point _cursorPosition{ 0, 0 };

	int _screenWidth, _screenHeight;

	int _playerId;

	MouseRect* _mouseRect;

	//单位管理器
	UnitManager* _unitManager;

	//游戏地图管理器
	GameManager* _gameManager;

	GridMap* _gridMap = nullptr;
	TMXTiledMap* _tiledMap = nullptr;

	void update(float delta);

private:

	Client * _client = nullptr;
	//LocalServer* _server = nullptr;

	void textFieldEvent(Ref *pSender, cocos2d::ui::TextField::EventType type);
	void animationInit();
	//void createFighterCallBack(Ref* pSender);
};

class ControlPanel :public Menu {
	friend class Factory;
public:
	CREATE_FUNC(ControlPanel);
	bool init();

	void createFighterCallBack(Ref* pSender);
	void createTankCallBack(Ref* pSender);
private:
	//children
	MenuItemImage* _fighter = nullptr;
	MenuItemImage* _tank = nullptr;
	Factory* _factory = nullptr;//当前和控制面板连接的factory，实现回调
	

	/*用mainScene初始化panel，用于代替init*/
	bool initWithScene(MainScene* scene);

};