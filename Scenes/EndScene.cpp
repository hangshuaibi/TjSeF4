#include "EndScene.h"
#include"Mainscene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* EndScene::createScene(bool flag)
{

	auto scene = Scene::create();

	auto layer = EndScene::create(flag);

	scene->addChild(layer);

	return scene;
}

bool EndScene::init(bool flag)
{
	//父类函数
	if (!Layer::init())
	{
		return false;
	}


	auto visibleSize = Director::getInstance()->getVisibleSize();
	//_screenWidth = visibleSize.width;
	//_screenHeight = visibleSize.height;
	//添加胜利or失败图片
	if (flag)
	{
		auto winimage = Sprite::create("scene/winimage.png");
		winimage->setScale(1);
		winimage->setPosition(visibleSize.width *0.5, visibleSize.height*0.6);
		addChild(winimage, 2);
	}
	else
	{
		auto loseimage = Sprite::create("scene/loseimage.png");
		loseimage->setScale(0.5);
		loseimage->setPosition(visibleSize.width *0.5, visibleSize.height*0.6);
		addChild(loseimage, 2);
	}
	//文字内容


	//创建菜单选项
	auto label1 = Label::createWithSystemFont("Back To The Menu", "Marker Felt", 15);
	auto LabelItem1 = MenuItemLabel::create(label1,
		CC_CALLBACK_1(EndScene::menuGoToStartMenu, this));
	LabelItem1->setPosition(visibleSize.width / 2, visibleSize.height *0.3);

	auto label2 = Label::createWithSystemFont("Quit", "Marker Felt", 15);
	auto LabelItem2 = MenuItemLabel::create(label2,
		CC_CALLBACK_0(EndScene::EndGame, this));
	LabelItem2->setPosition(visibleSize.width / 2, visibleSize.height *0.2);

	auto endmenu = Menu::create(LabelItem1, LabelItem2, NULL);
	endmenu->setPosition(Vec2::ZERO);
	addChild(endmenu);



	return true;
}
void EndScene::menuGoToStartMenu(Ref* pSender)
{
	//Director::getInstance()->replaceScene(ScenePool::preScene);
}
void EndScene::EndGame()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

}
