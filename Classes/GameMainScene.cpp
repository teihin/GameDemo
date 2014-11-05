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

	//创建地图层
	_tileMap = TMXTiledMap::create("./map/map.tmx");
	_tileMap->setTag(200);
	Size size = _tileMap->getTileSize();
	//拿到player位置
	TMXObjectGroup* pObjectGroup = _tileMap->getObjectGroup("obj");
	auto pPlayerObj = pObjectGroup->getObject("playerobj");
	Point position;
	position.x = pPlayerObj["x"].asInt();
	position.y = pPlayerObj["y"].asInt();
	this->addChild(_tileMap);

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
//	pPlayer->setPosition(Vec2(position.x,position.y));
	pPlayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pPlayer->m_nCurPosition = position;
	this->addChild(pPlayer);
	
	//移动摄像机视角,相当于移动主角坐标
	setViewPointCenter(position,0);
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

	//获取地图层，并移动地图
	TMXTiledMap* pMap = (TMXTiledMap*)getChildByTag(200);
	if (pMap)
	{
		if (nDir == JoystickEnum::DEFAULT)
		{
			this->unscheduleAllSelectors();
			return;
		}
			
		//获取方向类型，正方向快速移动，斜方向慢速移动
		if (nDir % 2 == 0) //正方向
		{
			//moveMapTimmer1(0.5);
			this->schedule(schedule_selector(GameMainScene::moveMapTimmer1, this), 0.2, kRepeatForever,0.001);
		}
		else
		{
			//moveMapTimmer2(0.5);
			this->schedule(schedule_selector(GameMainScene::moveMapTimmer2, this), 0.2, kRepeatForever,0.001);
		}
		
	}
}
void GameMainScene::setViewPointCenter(Point position, float dtTime)
{
	auto winSize = Director::getInstance()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
	y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
	auto actualPosition = Point(x, y);

	auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
	auto viewPoint = centerOfView - actualPosition;
	//_tileMap->setPosition(viewPoint);
	auto actionMove = MoveTo::create(dtTime, viewPoint);
	_tileMap->runAction(actionMove);
}

void GameMainScene::moveMapTimmer1(float dt)//正方向
{
	//获取主角指针
	Player* pPlayer = (Player*)getChildByTag(100);
	if (pPlayer)
	{
		//根据方向获取目标移动点坐标
		//Point positionNext; //下个点坐标
		switch (pPlayer->m_nDirect)
		{
		case FigureDirectionType::FDirRight:
		{
			pPlayer->m_nCurPosition.x += _tileMap->getTileSize().width;
			break;
		}
		case FigureDirectionType::FDirLeft:
		{
			pPlayer->m_nCurPosition.x -= _tileMap->getTileSize().width;
			break;
		}
		case FigureDirectionType::FDirUp:
		{
			pPlayer->m_nCurPosition.y += _tileMap->getTileSize().height;
			break;
		}
		case FigureDirectionType::FDirDown:
		{
			pPlayer->m_nCurPosition.y -= _tileMap->getTileSize().height;
			break;
		}
		default:
			return;
		}
		//pPlayer->m_nCurPosition = positionNext;
		setViewPointCenter(pPlayer->m_nCurPosition, 0.2);
	}
}
void GameMainScene::moveMapTimmer2(float dt)//斜方向
{
	//获取主角指针
	Player* pPlayer = (Player*)getChildByTag(100);
	if (pPlayer)
	{
		//根据方向获取目标移动点坐标
		//Point positionNext; //下个点坐标
		switch (pPlayer->m_nDirect)
		{
		case FigureDirectionType::FDirUpAndRight:
		{
			pPlayer->m_nCurPosition.x += _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y += _tileMap->getTileSize().height;
			break;
		}
		case FigureDirectionType::FDirLeftAndUp:
		{
			pPlayer->m_nCurPosition.x -= _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y += _tileMap->getTileSize().width;
			break;
		}
		case FigureDirectionType::FDirDownAndLeft:
		{
			pPlayer->m_nCurPosition.x -= _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y -= _tileMap->getTileSize().height;
			break;
		}
		case FigureDirectionType::FDirRightAndDown:
		{
			pPlayer->m_nCurPosition.x += _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y -= _tileMap->getTileSize().height;
			break;
		}
		default:
			return;
		}
		//pPlayer->m_nCurPosition = positionNext;
		setViewPointCenter(pPlayer->m_nCurPosition, 0.2);
	}
}