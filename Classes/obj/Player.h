#pragma once

#include "cocos2d.h"
#include "GameObjDef.h"
class Player : public cocos2d::Layer, public GObject
{
public:
	CREATE_FUNC(Player);
	virtual bool init();
public:
	std::string m_strCharString;	//设置角色字符串
	FigureDirectionType m_nDirect;	//当前方向
	FigureStateType		m_nState;	//当前状态


	cocos2d::Sprite* m_pPlayerSprite; //玩家精灵
public:
	void setCurState(FigureDirectionType nDirType, FigureStateType nStateType); //设置当前方向和状态，同时触发动画
private:	
	cocos2d::ActionInterval* getRunActionsFromSprite(std::string &strPath, FigureStateType nType, FigureDirectionType nDir);
	

};

