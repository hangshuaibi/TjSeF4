//GameMenu类的作用是显示一个菜单场景，让玩家选择“开始游戏”或是“游戏设置”
#include<cocos2d.h>
class GameMenu : public cocos2d ::Layer
{
public:
	
	/*
	静态函数，创造Scece场景
	建议返回实例类型的指针
	*/

	static cocos2d::Scene*createScene();
	
	/*
	init函数：完成初始化操作
	返回bool值
	*/

	virtual bool init();
	
	/*
	使用CREATE_FUNC宏来创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/

	CREATE_FUNC(GameMenu);
};