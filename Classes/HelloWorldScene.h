#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

// HelloWorld类继承cocos2d::Layer（层）
class HelloWorld : public cocos2d::Layer
{
private:
	TMXTiledMap * _tileMap; // 地图
	Sprite* _player; // 玩家
	int _screenWidth, _screenHeight; // 屏幕的宽度和高度
public:
	/**
	* 静态函数，创建Scene场景
	* 注意：在Cocos2d-x中没有Cocos2d-iPhone中的“id”,建议返回类实例的指针
	*/
	static cocos2d::Scene* createScene();
	/**
	* init函数，完成初始化操作。
	* 注意：init函数在Cocos2d-x中返回bool值，而不是返回Cocos2d-iPhone中的“id”
	*/
	virtual bool init();

	// 把玩家作为视觉中心来显示，让地图随玩家一起移动
	void setViewpointCenter(Vec2 position);

	// 选择器回调函数
	void menuCloseCallback(cocos2d::Ref* pSender);

	/**
	* 使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/
	CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
