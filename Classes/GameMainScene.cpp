#include "GameMainScene.h"
#include "Joystick.h"
#include "obj\Player.h"
USING_NS_CC;

Scene* GameMainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameMainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
bool GameMainScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
   Size visibleSize = Director::getInstance()->getVisibleSize();
   Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameMainScene::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	

	//创建摇杆层
	auto pController = Joystick::create("./gameui/rocker_bg.png", "./gameui/rocker_ball.png");
	pController->setAutoPosition(true);
	pController->onRun();
	pController->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//设置摇杆回调
	pController->onDirection = CC_CALLBACK_1(GameMainScene::mouthCallBack,this);
	this->addChild(pController);

	//添加主角层
	auto pPlayer = Player::create();
	pPlayer->setTag(100);
	pPlayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(pPlayer);
}
void GameMainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameMainScene::mouthCallBack(JoystickEnum nDir)
{
	//实时修改主角方向
	//获取主角
	Player* pPlayer = (Player*)getChildByTag(100);
	if (pPlayer)
	{
		pPlayer->setCurState((FigureDirectionType)nDir, FigureStateType::FStateRun);
	}
}