#pragma once

#include "cocos2d.h"
#include "GameObjDef.h"
class Player : public cocos2d::Layer, public GObject
{
public:
	CREATE_FUNC(Player);
	virtual bool init();
public:
	cocos2d::Sprite* m_pPlayerSprite; //Íæ¼Ò¾«Áé
	cocos2d::ActionInterval* getRunActionsFromSprite(std::string &strPath, FigureStateType nType, FigureDirectionType nDir);
};

