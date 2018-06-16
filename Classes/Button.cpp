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
	//createUnit

	//回位
	_buttonCopy->setPosition(getPosition());
	_buttonCopy->setVisible(false);
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