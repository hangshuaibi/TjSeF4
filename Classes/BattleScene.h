//
//  BattleScene.h
//  MX2
//
//  Created by apple on 14-5-8.
//
//

#ifndef __MX2__BattleScene__
#define __MX2__BattleScene__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

using namespace std;

// 游戏背景音乐
#define MUSIC_FILE        "background.mp3"
#define FIGHT_FILE        "fight.mp3"
#define SHOOT_FILE        "shoot.wav"
#define BOMB_FILE        "bomb.mp3"

// 游戏主场景
class BattleScene : public cocos2d::Layer
{
private:
	TMXTiledMap * _battleMap;  // 地图
	Sprite* _player;  // 玩家精灵
	
	TMXLayer* _collidable;  // 障碍层
	int _screenWidth, _screenHeight;  // 屏幕宽度和高度
	int _count;  // 游戏帧计数器
	
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
	virtual void onEnter();

	

	// 回调更新函数，该函数每一帧都会调用
	void update(float delta);
	/**
	* 使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/
	CREATE_FUNC(BattleScene);
};

#endif /* defined(__MX2__BattleSceneScene__) */
