#include "cocos2d.h"

USING_NS_CC;

class EndScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene(bool flag);

	virtual bool init(bool flag);

	static EndScene* create(bool flag) {
		EndScene* pRet = new EndScene();
		if (pRet && pRet->init(flag)) {
			pRet->autorelease();   //自动释放资源  
		}
		else {
			delete pRet;
			pRet = NULL;
		}
		return pRet;
	}

	void EndGame();

	void menuGoToStartMenu(Ref* pSender);
private:

	//int _screenWidth, _screenHeight;
};

