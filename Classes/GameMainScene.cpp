#include "GameMainScene.h"
#include "Joystick.h"
#include "obj\Player.h"
#include <math.h>
USING_NS_CC;

#define MOVE_SPEED_DEF	0.1	//Ĭ���ƶ��ٶȣ�ÿ��ô��ʱ���ƶ�һ�� ��λ��
float g_fMoveSpeed_Xie = 0; //б���ƶ��ٶ�
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

	//������ͼ��
	_tileMap = TMXTiledMap::create("./map/map1.tmx");
	_tileMap->setTag(200);
	Size size = _tileMap->getTileSize();
	//�õ�playerλ��
	TMXObjectGroup* pObjectGroup = _tileMap->getObjectGroup("obj");
	auto pPlayerObj = pObjectGroup->getObject("playerobj");
	Point position;
	position.x = pPlayerObj["x"].asInt();
	position.y = pPlayerObj["y"].asInt();
	this->addChild(_tileMap);

	//����ҡ�˲�
	auto pController = Joystick::create("./gameui/rocker_bg.png", "./gameui/rocker_ball.png");
	pController->setAutoPosition(true);
	pController->onRun();
	pController->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//����ҡ�˻ص�
	pController->onDirection = CC_CALLBACK_1(GameMainScene::mouthCallBack,this);
	this->addChild(pController);

	//������ǲ�
	auto pPlayer = Player::create();
	pPlayer->setTag(100);
//	pPlayer->setPosition(Vec2(position.x,position.y));
	pPlayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	pPlayer->m_nCurPosition = position;
	pPlayer->ShowGridBox(_tileMap->getTileSize().width);
	this->addChild(pPlayer);
	
	//�ƶ�������ӽ�,�൱���ƶ���������
	m_bCalling = false;
	setViewPointCenter(position,0);


	//����б���ƶ�ʱ��
	float fSpeed = _tileMap->getTileSize().width/MOVE_SPEED_DEF;
	//����б���ƶ�ʱ��
	g_fMoveSpeed_Xie = _tileMap->getTileSize().width / cosf(45) / fSpeed;
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
	//ʵʱ�޸����Ƿ���
	//��ȡ����
	Player* pPlayer = (Player*)getChildByTag(100);


	//��ȡ��ͼ�㣬���ƶ���ͼ
	TMXTiledMap* pMap = (TMXTiledMap*)getChildByTag(200);
	if (pMap)
	{
		//���֮ǰ�Ѿ����ڱ���״̬��ֻ��״̬�����������ص�
		if (!m_bCalling)
		{
			moveMapCallBack();
		}
		if (pPlayer)
		{
			pPlayer->setCurState((FigureDirectionType)nDir, FigureStateType::FStateRun);
		}


		//if (nDir == JoystickEnum::DEFAULT)
		//{
		//	this->unscheduleAllSelectors();
		//	return;
		//}
		//	
		////��ȡ�������ͣ�����������ƶ���б���������ƶ�
		//if (nDir % 2 == 0) //������
		//{
		//	//moveMapTimmer1(0.5);
		//	this->schedule(schedule_selector(GameMainScene::moveMapTimmer1, this), MOVE_SPEED_DEF, kRepeatForever,0.0001);
		//}
		//else
		//{
		//	//moveMapTimmer2(0.5);
		//	this->schedule(schedule_selector(GameMainScene::moveMapTimmer2, this), g_fMoveSpeed_Xie, kRepeatForever,0.0001);
		//}
		//
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
	auto callfun = CallFunc::create(this, callfunc_selector(GameMainScene::moveMapCallBack));
	auto seq = Sequence::create(actionMove, callfun, NULL);
	_tileMap->runAction(seq);
}

void GameMainScene::moveMapTimmer1(float dt)//������
{
	//��ȡ����ָ��
	Player* pPlayer = (Player*)getChildByTag(100);
	if (pPlayer)
	{
		//���ݷ����ȡĿ���ƶ�������
		//Point positionNext; //�¸�������
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
		setViewPointCenter(pPlayer->m_nCurPosition, MOVE_SPEED_DEF);
	}
}
void GameMainScene::moveMapTimmer2(float dt)//б����
{
	//��ȡ����ָ��
	Player* pPlayer = (Player*)getChildByTag(100);
	if (pPlayer)
	{
		//���ݷ����ȡĿ���ƶ�������
		//Point positionNext; //�¸�������
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
		setViewPointCenter(pPlayer->m_nCurPosition, g_fMoveSpeed_Xie);
	}
}


void GameMainScene::moveMapCallBack()
{
	//��ȡ����ָ��
	Player* pPlayer = (Player*)getChildByTag(100);
	if (pPlayer)
	{
		//���Ϊ����״̬���������ֱ������
		if (pPlayer->m_nState != FigureStateType::FStateRun)
		{
			m_bCalling = false;
			return;
		}

		m_bCalling = true;
		//���ݷ����ȡĿ���ƶ�������
		float fMoveTime = MOVE_SPEED_DEF;
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
		case FigureDirectionType::FDirUpAndRight:
		{
			pPlayer->m_nCurPosition.x += _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y += _tileMap->getTileSize().height;
			fMoveTime = g_fMoveSpeed_Xie;
			break;
		}
		case FigureDirectionType::FDirLeftAndUp:
		{
			pPlayer->m_nCurPosition.x -= _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y += _tileMap->getTileSize().width;
			fMoveTime = g_fMoveSpeed_Xie;
			break;
		}
		case FigureDirectionType::FDirDownAndLeft:
		{
			pPlayer->m_nCurPosition.x -= _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y -= _tileMap->getTileSize().height;
			fMoveTime = g_fMoveSpeed_Xie;
			break;
		}
		case FigureDirectionType::FDirRightAndDown:
		{
			pPlayer->m_nCurPosition.x += _tileMap->getTileSize().width;
			pPlayer->m_nCurPosition.y -= _tileMap->getTileSize().height;
			fMoveTime = g_fMoveSpeed_Xie;
			break;
		}
		default:
			return;
		}
		setViewPointCenter(pPlayer->m_nCurPosition, fMoveTime);
	}
}