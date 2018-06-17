#include "Button.h"
#include "MainScene.h"

BButton* BButton::create(MainScene* scene, Type type, std::string filename)
{
	BButton* ret = new BButton();
	if (ret&&ret->init(scene, type, filename))
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool BButton::init(MainScene* scene, Type type, std::string filename)
{
	_type = type;
	_buttonCopy = Sprite::create(filename);
	_unitManager = scene->_unitManager;

	assert(_unitManager != nullptr);
	assert(_buttonCopy != nullptr);
	scene->addChild(_buttonCopy, 5);
	_buttonCopy->setVisible(false);

	return this->initWithFile(filename);
}

void BButton::onPress()
{
	_buttonCopy->setPosition(getPosition());
	_buttonCopy->setVisible(true);
}

void BButton::onRelease()
{
	//得到按钮副本被拖到的位置
	auto pos = _buttonCopy->getPosition();

	//回位
	_buttonCopy->setPosition(getPosition());
	_buttonCopy->setVisible(false);

	if (!_unitManager->isStart())
	{
		_unitManager->notice(Notice::WAIT_FOR_START);
		return;
	}

	//createUnit
	if (!_unitManager->canCreate(_type))//没钱的话
	{
		_unitManager->notice(Notice::NO_ENOUGH_MONEY);
		return;
	}
	static GridMap* map = _unitManager->_gridMap;
	auto truePos = _unitManager->_tiledMap->convertToNodeSpace(pos);

	if (!map->isGridValid(map->getGrid(truePos)))
	{
		_unitManager->notice(Notice::OCCUPIED_POSITION);//建造地不合法的话
		return;
	}

	_unitManager->costForCreate(_type);//花钱
	_unitManager->localCreateUnit(_type, truePos);//发消息
	
}

bool BButton::isTouched(Touch* touch)
{
	if (Rect(0, 0, getContentSize().width,
		getContentSize().height).containsPoint
		(convertToNodeSpace(touch->getLocation())))
	{
		return true;
	}
	return false;
}

