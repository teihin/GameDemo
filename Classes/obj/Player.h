#pragma once

#include "cocos2d.h"
#include "GameObjDef.h"
class Player : public cocos2d::Layer, public GObject
{
public:
	CREATE_FUNC(Player);
	virtual bool init();
public:
	std::string m_strCharString;	//���ý�ɫ�ַ���
	FigureDirectionType m_nDirect;	//��ǰ����
	FigureStateType		m_nState;	//��ǰ״̬
	cocos2d::Point	m_nCurPosition;			//��ǰ����

	cocos2d::Sprite* m_pPlayerSprite; //��Ҿ���

	int m_nGridWidth;	//�������
public:
	void ShowGridBox(int nGridW);
	void setCurState(FigureDirectionType nDirType, FigureStateType nStateType); //���õ�ǰ�����״̬��ͬʱ��������
private:	
	cocos2d::ActionInterval* getRunActionsFromSprite(std::string &strPath, FigureStateType nType, FigureDirectionType nDir);
	

};

