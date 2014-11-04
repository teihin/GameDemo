#include "Player.h"

USING_NS_CC;

bool Player::init()
{
	if (!Layer::init())
		return false;
	Size vSize = Director::getInstance()->getVisibleSize();

	//加载资源
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("./player/char_11001.plist");

	//加上主角精灵试试
	m_pPlayerSprite = Sprite::createWithSpriteFrameName("char_11001_1_4_00.png");
	//m_pPlayerSprite->setPosition(Vec2(vSize.width / 2, vSize.height / 2));
	this->addChild(m_pPlayerSprite);
	m_strCharString = "char_11001";
	//m_pPlayerSprite->runAction(RepeatForever::create(getRunActionsFromSprite(m_strCharString, FStateRun, FDirRight)));
	setCurState(FigureDirectionType::FDirLeft, FigureStateType::FStateWalk);
	return true;
}

static float getFrameRate(FigureStateType stateType, int type)
{
	float frameRate = 0;

	if (type == 1) //人
	{
		if (stateType == FStateStand)
		{
			frameRate = 1 / 3.f;
		}
		else if (stateType == FStateWalk)
		{
			frameRate = 0.6 / 16.f;
		}
		else if (stateType == FStateRun)
		{
			frameRate = 0.6 / 16.f;
		}
		else if (stateType == FStateAttack)
		{
			frameRate = 1 / 8.f;
		}
		else if (stateType == FStateCaster)
		{
			frameRate = 1 / 8.f;
		}
		else if (stateType == FStateHurt)
		{
			frameRate = 1 / 8.f;
		}
		else if (stateType == FStateDeath)
		{
			frameRate = 1 / 4.f;
		}

	}

	if (type == 2) //怪
	{
		if (stateType == FStateStand)
		{
			frameRate = 1 / 5.f;
		}
		else if (stateType == FStateWalk)
		{
			frameRate = 0.6 / 8.f;
		}
		else if (stateType == FStateRun)
		{
			frameRate = 0.6f / 8.f;
		}
		else if (stateType == FStateAttack)
		{
			frameRate = 1 / 8.f;
		}
		else if (stateType == FStateCaster)
		{
			frameRate = 1 / 8.f;
		}
		else if (stateType == FStateHurt)
		{
			frameRate = 1 / 8.f;
		}
		else if (stateType == FStateDeath)
		{
			frameRate = 1 / 8.f;
		}
	}
	return frameRate;
}


ActionInterval* Player::getRunActionsFromSprite(std::string &strPath, FigureStateType nType, FigureDirectionType nDir)
{
	//CCArray* array = CCArray::createWithCapacity(8);
	Vector<SpriteFrame*> array;
	int flag = 0;
	do
	{
		int dirType = nDir;
		if (8 > dirType && dirType > 4)
		{
			dirType = 8 - dirType;
		}

		auto string = String::createWithFormat("%s_%d_%d_%02d.png", strPath.c_str(), nType, dirType, flag);
		
		//std::string string;
		//sprintf(string, "%s_%d_%d_%02d.png", strPath.c_str(), nType, dirType, flag);
		auto spriteFrame = SpriteFrameCache::getInstance()->spriteFrameByName(string->getCString());
		CC_BREAK_IF(spriteFrame == NULL);
		array.pushBack(spriteFrame);
		flag++;
	} while (1);

	CCAnimation* movie = CCAnimation::createWithSpriteFrames(array,getFrameRate(nType,1));
	CCAnimate* animate = CCAnimate::create(movie);
	return animate;
}

void Player::setCurState(FigureDirectionType nDirType, FigureStateType nStateType)
{
	//如果方向和状态都不变，为避免卡顿不调整状态
	if (nDirType == m_nDirect&& nStateType == m_nState)
		return;


	m_nDirect = nDirType;
	m_nState = nStateType;

	//由于图片没有左方向，所有左方向的动画都需要翻转180度
	if (m_nDirect == FDirLeft || m_nDirect == FDirLeftAndUp || m_nDirect == FDirDownAndLeft)
	{
		m_pPlayerSprite->setRotationY(180);
	}
	else
	{
		m_pPlayerSprite->setRotationY(0);
	}

	//停止之前动画启动新状态
	m_pPlayerSprite->stopAllActions();
	m_pPlayerSprite->runAction(RepeatForever::create(getRunActionsFromSprite(m_strCharString, m_nState, m_nDirect)));
}