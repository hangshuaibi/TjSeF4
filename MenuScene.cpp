#include"MenuScene.h"
#include"AudioControlScene.h"
#include"MainScene.h"
#include"ui/CocosGUI.h"
#include"SimpleAudioEngine.h"
#include"cocos2d.h"
#include"HelloWorldScene.h"
USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
#define MUSIC_FILE "bgm.wav"

Scene* GameMenu::createScene()
{ 
	auto scene = Scene::create();
	auto layer = GameMenu::create();
	scene->addChild(layer);
	return scene;
}

char* EncodeToUTF8(const char* mbcsStr)
{
	wchar_t*  wideStr;
	char*     utf8Str;
	int       charLen;
	charLen = MultiByteToWideChar(936, 0, mbcsStr, -1, NULL, 0);
	wideStr = (wchar_t*)malloc(sizeof(wchar_t)*charLen);
	MultiByteToWideChar(936, 0, mbcsStr, -1, wideStr, charLen);
	charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);
	utf8Str = (char*)malloc(charLen);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);
	free(wideStr);
	return utf8Str;
}


bool GameMenu::init()
{

	//调用父类的init方法

	if (!Layer::init())
	{
		return false;
	}

	//预加载背景音乐
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);

	//播放背景音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE, true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);



	auto sprite = Sprite::create("fate.jpg");
	
	
	//获得设备可见视图大小

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	sprite->setScale(1);
	this->addChild(sprite, 0);

	//“开始游戏”按钮

	auto start_button = Button::create("button.png");
	start_button->setScale(1.7);
	start_button->setTitleText(EncodeToUTF8("开始游戏"));
	start_button->setTitleFontName("微软雅黑");
	start_button->setTitleFontSize(11);
	start_button->setPosition(Vec2(visibleSize.width/2, visibleSize.height*0.7));
	start_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{

			//切换到mainscene场景

			auto transition = TransitionSlideInL::create(2.0, MainScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_button);

	//“游戏设置”按钮

	auto set_button = Button::create("button.png");
	set_button->setScale(1.7f);
	set_button->setTitleText(EncodeToUTF8("游戏设置"));
	set_button->setTitleFontName("微软雅黑");
	set_button->setTitleFontSize(11);
	set_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.55));
	set_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{

			//切换到AudioControl场景

			auto transition = TransitionSlideInL::create(2.0, AudioControl::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(set_button);

	//"退出游戏"按钮

	auto close_button = Button::create("button.png");
	close_button->setScale(1.7);
	close_button->setTitleText(EncodeToUTF8("退出游戏"));
	close_button->setTitleFontName("微软雅黑");
	close_button->setTitleFontSize(11);
	close_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.4));
	close_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			Director::getInstance()->end();
			exit(0);
		}
	});
	this->addChild(close_button);

	//组名

	auto banner_text = Text::create(EncodeToUTF8("四剑客"), "Arial-BoldMT", 32);
	banner_text->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.15));
	this->addChild(banner_text);





	return true;
}








