#ifndef __GameObjDef__
#define __GameObjDef__

#include <string>

typedef enum
{
	FDirUp = 8,
	FDirLeftAndUp = 7,
	FDirLeft = 6,
	FDirDownAndLeft = 5,
	FDirDown = 4,
	FDirRightAndDown = 3,
	FDirRight = 2,
	FDirUpAndRight = 1,
	FDirNone = 0
}FigureDirectionType;

typedef enum
{
	FStateDeath = 7,
	FStateHurt = 6,
	FStateCaster = 5,
	FStateAttack = 4,
	FStateRun = 3,
	FStateWalk = 2,
	FStateStand = 1,
	FStateNone = 0
}FigureStateType;

typedef struct GObject
{
	std::string strName;
};

#endif