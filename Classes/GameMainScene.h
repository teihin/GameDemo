#pragma once
#include "cocos2d.h"
#include "Joystick.h"

class GameMainScene: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameMainScene);
public:
	void mouthCallBack(JoystickEnum nDir); //鼠标方向回调

	void setViewPointCenter(Point position,float dtTime);//设置摄像机中心位置
	void moveMapTimmer1(float dt);
	void moveMapTimmer2(float dt);
public:
	cocos2d::TMXTiledMap* _tileMap;
};

