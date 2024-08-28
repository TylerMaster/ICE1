/***************************
Where Is Poppy
2024-05-21

wip_monster.h

******************************/
#ifndef WIP_MONSTER_H
#define WIP_MONSTER_H

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "schedule.h"
#include "decals.h"

//=========================
//--Monster's Anim Events--
//=========================
#define PDRONE_FIRE_SPIKE  ( 1 )
#define PDRONE_MELEE_LEFT  ( 2 )
#define PDRONE_MELEE_RIGHT ( 4 )
#define PDRONE_MELEE_BOTH  ( 6 )
#define PDRONE_RELOAD     ( 7 )

//=========================
//--Monster's Extra Defs---
//=========================

//-Body Groups
#define BODYGROUP_BODY              1
#define BODYGROUP_SPIKES            2

//-Spike Groups
#define BODY_NO_SPIKES              0
#define BODY_SIX_SPIKES             1
#define BODY_FIVE_SPIKES            2
#define BODY_FOUR_SPIKES            3
#define BODY_THREE_SPIKES           4
#define BODY_TWO_SPIKES             5
#define BODY_ONE_SPIKES             6

#endif
