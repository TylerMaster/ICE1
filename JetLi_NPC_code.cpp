#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "soundent.h"
#include "schedule.h"
#include "skill.h"
#include "monsters.h"
#include "jetli_thespecialists.h"
#include <cstdlib> 


#define TASK_JETLI_CHASE_ENEMY (LAST_COMMON_TASK + 1)
#define SCHED_JETLI_CHASE_ENEMY (LAST_COMMON_SCHEDULE + 1)


#define TASK_JETLI_IDLE (LAST_COMMON_TASK + 2)
#define SCHED_JETLI_IDLE (LAST_COMMON_SCHEDULE + 2)

#define TASK_JETLI_LEAP (LAST_COMMON_TASK + 3)
#define SCHED_JETLI_LEAP (LAST_COMMON_SCHEDULE + 3)

#define TASK_JETLI_SPECIAL_CHASE (LAST_COMMON_TASK + 4)
#define SCHED_JETLI_SPECIAL_CHASE (LAST_COMMON_SCHEDULE + 4)

#define TASK_GET_PATH_TO_LOCATION (LAST_COMMON_TASK + 5)

#define TASK_JETLI_SPECIAL_FLIP (LAST_COMMON_TASK + 6)

Task_t tlJetLiChase[] = {
	{ TASK_GET_PATH_TO_ENEMY, 0.0f },
	{ TASK_RUN_PATH, 0.0f },
	{ TASK_WAIT, 0.0f },
};

Task_t tlJetLiIdle[] = {
	{ TASK_STAND, 0.0f },
	{ TASK_WAIT, 0.0f },
};

Schedule_t slJetLiIdle[] = {
	{
		tlJetLiIdle,
		ARRAYSIZE(tlJetLiIdle),
		0,
		0,
		"Jet Li Idle"
	}
};

Schedule_t slJetLiChase[] = {
	{
		tlJetLiChase,
		ARRAYSIZE(tlJetLiChase),
		bits_COND_NEW_ENEMY |
		bits_COND_SMELL_FOOD,
		0,
		"Jet Li Chase"
	}
};

Task_t tlJetLiLeap[] = {
	{ TASK_PLAY_SEQUENCE, (float)ACT_LEAP },
	
};

Schedule_t slJetLiLeap[] = {
	{
		tlJetLiLeap,
		ARRAYSIZE(tlJetLiLeap),
		0,
		0,
		"Jet Li Leap"
	}
};

Task_t tlJetLiSpecialChase[] =
{
	{ TASK_GET_PATH_TO_LOCATION, 0 },
	{ TASK_RUN_PATH, 0 },
	{ TASK_WAIT_FOR_MOVEMENT, 0 },
	{ TASK_JETLI_SPECIAL_FLIP, 0 },
	{ TASK_WAIT, 0.0f },
};
Schedule_t slJetLiSpecialChase[] = {
	{
		tlJetLiSpecialChase,
		ARRAYSIZE(tlJetLiSpecialChase),
		bits_COND_NEW_ENEMY,
		0,
		"Jet Li Special Chase"
	}
};

class CJetLi : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	void PrescheduleThink() override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	Schedule_t* GetSchedule() override;
	Schedule_t* GetScheduleOfType(int Type) override;
	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	Vector m_vecGoal;
	//void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;
	float hopFinishTime;
	
	
	CUSTOM_SCHEDULES;

private:
	int m_iRunSequence;
	int m_iWalkSequence;
	int m_iIdleSequence;
	int m_iLeapSequence;
	int m_iFlipSequence;
};


LINK_ENTITY_TO_CLASS(jetli_thespecialists, CJetLi);

DEFINE_CUSTOM_SCHEDULES(CJetLi)
{
	slJetLiChase,
		slJetLiIdle,
		slJetLiLeap,
		slJetLiSpecialChase,
};

IMPLEMENT_CUSTOM_SCHEDULES(CJetLi, CBaseMonster);


void CJetLi::Spawn()
{
	Precache();

	SET_MODEL(ENT(pev), "models/JetLijapan/JetLijapan.mdl");
	UTIL_SetSize(pev, Vector(-24.0f, -24.0f, -38.0f), Vector(24.0f, 24.0f, 50.0f));
	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_RED;
	pev->health = 100;
	pev->view_ofs = Vector(0.0f, -10.0f, 110.0f);
	m_flFieldOfView = 0.0; // How far he can see.
	MonsterInit();
	
	m_afCapability = bits_CAP_DUCK | bits_CAP_JUMP | bits_CAP_RANGE_ATTACK1 | bits_CAP_MELEE_ATTACK1 | bits_CAP_MELEE_ATTACK2;
	m_MonsterState = MONSTERSTATE_NONE;
	m_iRunSequence = LookupSequence("run2");
	m_iIdleSequence = LookupSequence("standard_idle");
	m_iLeapSequence = LookupSequence("standard_jump");
	m_iFlipSequence = LookupSequence("wheel_f");
}

int CJetLi::Classify()
{
	return CLASS_HUMAN_MILITARY; // Or whatever classification is appropriate for your game
}

void CJetLi::PrescheduleThink()
{
	
	
	Vector vecToEnemy;
	float flDist;

	
	CBaseEntity* nextTarget = nullptr;
	

	if (!m_hEnemy || !m_hEnemy->pev)
	{
		while (nextTarget = UTIL_FindEntityInSphere(nextTarget, pev->origin, 512.0f)){
			if (nextTarget->IsPlayer())
			{
				m_hEnemy = nextTarget;
			}
			
		}
		//ALERT(at_console, "No valid enemy!\n");
		return;
	}

	vecToEnemy = m_hEnemy->pev->origin - pev->origin; // direction
	vecToEnemy.z = 0;
	//flDist = vecToEnemy.Length();                    // distance
	
	
	m_vecGoal = pev->origin + vecToEnemy * 0.5;
	m_vecGoal.z = pev->origin.z;

	ALERT(at_console, "Player: x=%.2f, y=%.2f, z=%.2f\n", m_hEnemy->pev->origin.x, m_hEnemy->pev->origin.y, m_hEnemy->pev->origin.z);
	ALERT(at_console, "vecGoal: x=%.2f, y=%.2f, z=%.2f\n", m_vecGoal.x, m_vecGoal.y, m_vecGoal.z);
	//ALERT(at_console, "flDist = %.2f\n", flDist);
	
	CBaseMonster::PrescheduleThink();
}

void CJetLi::Precache()
{

	PRECACHE_MODEL("models/JetLijapan/JetLijapan.mdl"); //Loads the NPC model in the game

	PRECACHE_SOUND("JetLijapan/jump.wav");
	PRECACHE_SOUND("JetLijapan/kick.wav");
	PRECACHE_SOUND("JetLijapan/punch.wav");
	PRECACHE_SOUND("JetLijapan/step.wav");
}




void CJetLi::SetYawSpeed(void) {
	pev->yaw_speed = 90;
}




void CJetLi::HandleAnimEvent(MonsterEvent_t* pEvent)
{


	ALERT(at_console,
		"JetLi AnimEvent: event=%d frame=%d\n",
		pEvent->event);

	switch (pEvent->event)
	{
	case JETLI_PUNCH:
	{
						
							  CBaseEntity *pHurt = CheckTraceHullAttack(85, 20, DMG_SLASH);

							 
							  if (pHurt)
							  {
								  EMIT_SOUND_DYN(ENT(pev), CHAN_BODY, "JetLijapan/punch.wav", 1.0, ATTN_NORM, 0, 100);
								  pHurt->pev->punchangle.y = 15;
								  pHurt->pev->punchangle.x = 8;
								  pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_up * -100;
							  }
							  break;
	}
	case JETLI_KICK:
	{
					  
							   CBaseEntity *pHurt = CheckTraceHullAttack(85, 20, DMG_SLASH);

							   if (pHurt)
							   {
								   EMIT_SOUND_DYN(ENT(pev), CHAN_BODY, "JetLijapan/kick.wav", 1.0, ATTN_NORM, 0, 100);
								   pHurt->pev->punchangle.y = -15;
								   pHurt->pev->punchangle.x = 8;
								   pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_up * -100;
							   }
							   break;
	}
	case JETLI_RUN:
	{
					  
					  EMIT_SOUND_DYN(ENT(pev), CHAN_BODY, "JetLijapan/step.wav", 1.0, ATTN_NORM, 0, 100);
					  
					  break;
	}
	case JETLI_JUMP:
	{
					   pev->velocity = pev->velocity + gpGlobals->v_up * 200 + gpGlobals->v_forward * 100;
					   EMIT_SOUND_DYN(ENT(pev), CHAN_BODY, "JetLijapan/jump.wav", 1.0, ATTN_NORM, 0, 100);
					   
					   break;
	}
	case JETLI_FLIP:
	{
					   pev->velocity = pev->velocity + gpGlobals->v_up * 400 + gpGlobals->v_forward * 200;
					   EMIT_SOUND_DYN(ENT(pev), CHAN_BODY, "JetLijapan/jump.wav", 1.0, ATTN_NORM, 0, 100);
					  
					   break;
	}
	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}

}

Schedule_t* CJetLi::GetSchedule() {

	
	
	switch (m_MonsterState)
	{
	
	case MONSTERSTATE_COMBAT:
	{

								
								if (!m_hEnemy || m_hEnemy->pev->deadflag != DEAD_NO)
								{
									
									return GetScheduleOfType(SCHED_JETLI_CHASE_ENEMY);
								}

								
									if ((m_hEnemy->pev->origin - pev->origin).Length() >= 512)
									{
										return GetScheduleOfType(SCHED_JETLI_SPECIAL_CHASE);
									}
								
								if ((m_hEnemy->pev->origin - pev->origin).Length() <= 100)
								{
									if (HasConditions(bits_COND_CAN_MELEE_ATTACK1)){

										switch (RANDOM_LONG(0, 1))
										{
										case 0:
											return GetScheduleOfType(SCHED_MELEE_ATTACK1);
											break;
										case 1:
											return GetScheduleOfType(SCHED_MELEE_ATTACK2);
											break;
										}
									}
								}
								if ((m_hEnemy->pev->origin - pev->origin).Length() <= 512){
									return GetScheduleOfType(SCHED_JETLI_CHASE_ENEMY);
								}
								
								
	}
	default:
	{
			   ALERT(at_aiconsole, "Invalid State for GetSchedule!\n");
			   break;
	}
	}
	return CBaseMonster::GetSchedule();
}

Schedule_t* CJetLi::GetScheduleOfType(int Type)
{
	switch (Type)
	{
	case SCHED_JETLI_CHASE_ENEMY:
		return &slJetLiChase[0];
		break;
		
	case SCHED_JETLI_IDLE:

		return &slJetLiIdle[0];
		break;
	
		
	case SCHED_JETLI_LEAP:
		return &slJetLiLeap[0];
		break;

	case SCHED_JETLI_SPECIAL_CHASE:
		return &slJetLiSpecialChase[0];
		break;

	default:
		ALERT(at_aiconsole, "Invalid State for GetScheduleOfType!\n");
	}
	return CBaseMonster::GetScheduleOfType(Type);
}

void CJetLi::RunTask(Task_t* pTask)
{
	
	
	switch (pTask->iTask)
	{
	case TASK_GET_PATH_TO_LOCATION:
		if (FRouteClear()) 
		{

			TaskComplete();
		}
		else
		{

		}
		break;

	case TASK_STAND:
		if (m_fSequenceFinished)
		{
			TaskComplete();
		}
		CBaseMonster::RunTask(pTask);
		break;
	case TASK_RUN_PATH:

		if (m_fSequenceFinished)
		{
			TaskComplete();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;

	case TASK_JETLI_LEAP:

		if (m_fSequenceFinished)
		{
			TaskComplete();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;
		
	case TASK_JETLI_SPECIAL_FLIP:

		if (m_fSequenceFinished)
		{
			TaskComplete();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;
	default:
		CBaseMonster::RunTask(pTask);
		break;
	}
}

void CJetLi::StartTask(Task_t* pTask)
{
	
	
	
	
	switch (pTask->iTask)
	{
	case TASK_GET_PATH_TO_LOCATION:
	{
									  
									 


									  if (BuildRoute(m_vecGoal, bits_MF_TO_LOCATION, NULL))
										  {
											  
											  TaskComplete();
											  
										  }
									  else{
										  ALERT(at_console,
											  "BuildRoute FAILED");
									  }
										 
									  
									  break;

	}

	case TASK_JETLI_IDLE:
		if (m_iIdleSequence != -1)
		{
			pev->sequence = m_iIdleSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		
		break;
	
	case TASK_JETLI_CHASE_ENEMY:
		// Start the running animation

		if (m_iRunSequence != -1)
		{
			pev->sequence = m_iRunSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		
		break;
	case TASK_JETLI_LEAP:
		if (m_iLeapSequence != -1)
		{
			pev->sequence = m_iLeapSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
			
		break;

	case TASK_JETLI_SPECIAL_FLIP:
		if (m_iFlipSequence != -1)
		{
			pev->sequence = m_iFlipSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}

		break;
	default:
		CBaseMonster::StartTask(pTask);
		break;
	}
}
