#include"cocos2d.h"

//AudioControl类用来设置背景音乐和声音特效

class AudioControl :public cocos2d::Layer
{
public:
	
	/*
	静态函数，创造Scene场景
	建议返回类实例的指针
	*/
	
	static cocos2d::Scene* createScene();
	
	/*
	init函数，完成初始化操作
	注意：init函数在cocos2d-x中返回bool值
	*/

	virtual bool init();

	/*
	使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/

	CREATE_FUNC(AudioControl);
};