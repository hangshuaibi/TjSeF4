#include "GameManager.h"
#include "MainScene.h"

USING_NS_CC;

GameManager* GameManager::create(MainScene* mainScene)
{
	GameManager *gameManager = new GameManager();
	if (gameManager && gameManager->initWithScene(mainScene))
	{
		gameManager->autorelease();
		return gameManager;
	}
	CC_SAFE_DELETE(gameManager);
	return nullptr;
}

bool GameManager::initWithScene(MainScene* mainScene)
{
	_mainScene = mainScene;
	return true;
}
void GameManager::focusOnBase()
{
	float duration = 0.1;
	Vec2 position = Vec2(100, 100);      //暂定的基地的位置

	int x = MAX(position.x, _mainScene->_screenWidth / 2);
	int y = MAX(position.y, _mainScene->_screenHeight / 2);

	// 不让显示区域超出地图的边界

	x = MIN(x, (_mainScene->_tiledMap->getMapSize().width *
		_mainScene->_tiledMap->getTileSize().width - _mainScene->_screenWidth / 2));
	y = MIN(y, (_mainScene->_tiledMap->getMapSize().height *
		_mainScene->_tiledMap->getTileSize().height - _mainScene->_screenHeight / 2));

	// 实际移动的位置坐标
	Point actualPosition = Point(x, y);

	// 屏幕中心位置坐标
	Point centerOfView = Point(MainScene::_screenWidth / 2, MainScene::_screenHeight / 2);

	// 计算中心位置和实际移动的位置两点之间的差值
	Point viewPoint = centerOfView - actualPosition;
	//Point viewPoint = position;
	this->_mainScene->_tiledMap->setPosition(viewPoint);

	// 重新设置屏幕位置
	//this->_mainScene->runAction(MoveTo::create(duration, viewPoint));
}
void GameManager::scrollMap()
{
	//瓦片地图的位置
	auto mapCenter = _mainScene->_tiledMap->getPosition();  

	auto visibleSize = Director::getInstance()->getVisibleSize();           
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//水平方向和垂直方向的移动速度比率
	int horizontalRate = 0, verticalRate = 0;                                        


	//判断鼠标x坐标是否在最右边区域
	if (origin.x + visibleSize.width - BOX_EDGE_WITDH_SMALL < _mainScene->_crusorPosition.x)
	{
		++horizontalRate;
	}

	//判断鼠标x坐标是否在次右边区域
	if (origin.x + visibleSize.width - BOX_EDGE_WITDH < _mainScene->_crusorPosition.x)
	{
		++horizontalRate;
	}

	//判断鼠标x坐标是否在最左边区域
	if (origin.x + BOX_EDGE_WITDH_SMALL > _mainScene->_crusorPosition.x)
	{
		--horizontalRate;
	}

	//判断鼠标x坐标是否在次左边区域
	if (origin.x + BOX_EDGE_WITDH > _mainScene->_crusorPosition.x)
	{
		--horizontalRate;
	}





		//判断鼠标y坐标是否在最上边区域
	if (origin.y + visibleSize.height - BOX_EDGE_WITDH_SMALL < _mainScene->_crusorPosition.y)
	{
		++verticalRate;
	}

	//判断鼠标y坐标是否在次上边区域
	if (origin.y + visibleSize.height - BOX_EDGE_WITDH < _mainScene->_crusorPosition.y)
	{
		++verticalRate;
	}

	//判断鼠标y坐标是否在最下边区域
	if (origin.y + BOX_EDGE_WITDH_SMALL > _mainScene->_crusorPosition.y)
	{
		--verticalRate;
	}

	//判断鼠标y坐标是否在次下边区域
	if (origin.y + BOX_EDGE_WITDH > _mainScene->_crusorPosition.y)
	{
		--verticalRate;
	}




	Vec2 scroll(0, 0);
	
	scroll += Vec2(-SCROLL_LENGTH, 0)*horizontalRate;
	scroll += Vec2(0, -SCROLL_LENGTH)*verticalRate;

	//移动后瓦片地图的位置
	mapCenter += scroll;        

	//判断移动后是否出界
	if (_mainScene->_tiledMap->getBoundingBox().containsPoint((-scroll) + Director::getInstance()->getVisibleSize())
		&& _mainScene->_tiledMap->getBoundingBox().containsPoint(-scroll))

	//移动地图
		_mainScene->_tiledMap->setPosition(mapCenter);
}
