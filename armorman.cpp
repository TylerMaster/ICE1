#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "soundent.h"
#include "schedule.h"
#include "skill.h"
#include "monsters.h"
#include <cstdlib> 

#define ARMORMAN_SHOTGUN_OFFSET Vector(0.0f, 0.0f, 55.0f)

#define TASK_ARMORMAN_CHASE_ENEMY (LAST_COMMON_TASK + 1)
#define SCHED_ARMORMAN_CHASE_ENEMY (LAST_COMMON_SCHEDULE + 1)


#define SCHED_ARMORMAN_RELOAD (LAST_COMMON_SCHEDULE + 2)

#define SCHED_ARMORMAN_SHIELD (LAST_COMMON_SCHEDULE + 3)
#define TASK_ARMORMAN_SHIELD (LAST_COMMON_TASK + 2)

#define SCHED_ARMORMAN_WALK (LAST_COMMON_SCHEDULE + 4)
#define TASK_ARMORMAN_WALK (LAST_COMMON_TASK + 3)

#define SCHED_ARMORMAN_TURN (LAST_COMMON_SCHEDULE + 5)
#define TASK_ARMORMAN_TURN (LAST_COMMON_TASK + 4)
#define TASK_GET_PATH_TO_LOCATION (LAST_COMMON_TASK + 5)



//constexpr int ARMORMAN_AE_SHOOT = 3;


//Chase sequence

Task_t tlArmorManChase[] = {
	{ TASK_GET_PATH_TO_ENEMY, 0.0f },
	{ TASK_RUN_PATH, 0.0f },
	{ TASK_WAIT, 0.0f },
};

Schedule_t slArmorManChase[] = {
	{
		tlArmorManChase,
		ARRAYSIZE(tlArmorManChase),
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_RANGE_ATTACK1,
		0,
		"ArmorMan Chase"
	}
};

//Reload sequence
Task_t tlArmorManReload[] = {
	{ TASK_STOP_MOVING, 0.0f },
	{ TASK_FACE_ENEMY, 0.0f },
	{ TASK_PLAY_SEQUENCE, (float)ACT_RELOAD },

};

Schedule_t slArmorManReload[] {

	{
		tlArmorManReload,
		ARRAYSIZE(tlArmorManReload),
		bits_COND_HEAVY_DAMAGE | bits_COND_HEAR_SOUND,
		bits_SOUND_DANGER,
		"ArmorMan Reload"
	}
};

//Shield sequence
Task_t tlArmorManShield[] =
{
	{ TASK_STOP_MOVING, 0.0f },
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY, (float)ACT_EXCITED },
	
};

Schedule_t slArmorManShield[] =
{
	{
		tlArmorManShield,
		ARRAYSIZE(tlArmorManShield),
		0,
		0,
		"ArmorMan Shield"
	}
};

//Patrol
Task_t tlArmorManWalk[] = 
{
	{ TASK_GET_PATH_TO_LOCATION, 0 },
	{ TASK_WALK_PATH, 0 },
	{ TASK_WAIT_FOR_MOVEMENT, 0 },

};

Schedule_t slArmorManWalk[] = {
	{
		tlArmorManWalk,
		ARRAYSIZE(tlArmorManWalk),
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_RANGE_ATTACK1,
		0,
		"ArmorMan Walk"
	}
};

//Patrol Turn RIGHT
Task_t tlArmorManTurn[] =
{
	{ TASK_PLAY_SEQUENCE, (float)ACT_TURN_RIGHT },
	{ TASK_WAIT, 0.0f },
};

Schedule_t slArmorManTurn[] = {
	{
		tlArmorManTurn,
		ARRAYSIZE(tlArmorManTurn),
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_RANGE_ATTACK1,
		0,
		"ArmorMan Turn"
	}
};

class CArmorMan : public CBaseMonster
{
public:
	void Spawn() override;
	void Precache() override;
	void SetYawSpeed() override;
	int Classify() override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;
	void HandleAnimEvent(MonsterEvent_t* pEvent) override;
	Schedule_t* GetSchedule() override;
	Schedule_t* GetScheduleOfType(int Type) override;
	void Shoot(void);

	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	BOOL CArmorMan::CheckAmmo1();
	BOOL CArmorMan::CheckWallDistance();
	void CArmorMan::TurnRight();
	void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;
	CUSTOM_SCHEDULES;

private:
	int m_iRunSequence;
	int m_iWalkSequence;
	int m_iTurnRightSequence;
	int m_iReloadSequence;
	int m_iShieldSequence;
	int m_flNextShieldTime;
	int m_flNextHealthTime;
	int m_distance;
	int m_flNextDisCheckTime;
	BOOL m_healthCycle;
};

LINK_ENTITY_TO_CLASS(monster_armorman, CArmorMan);

DEFINE_CUSTOM_SCHEDULES(CArmorMan)
{
	slArmorManChase,
	slArmorManReload,
	slArmorManShield,
	slArmorManWalk,
	slArmorManTurn
};

IMPLEMENT_CUSTOM_SCHEDULES(CArmorMan, CBaseMonster);


void CArmorMan::Spawn()
{
	Precache();
	SET_MODEL(ENT(pev), "models/armorman/armorman.mdl");

	UTIL_SetSize(pev, Vector(-24.0f, -24.0f, 0.0f), Vector(24.0f, 24.0f, 108.0f));
	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_RED;
	pev->health = gSkillData.armormanHealth;

	pev->view_ofs = Vector(0.0f, -10.0f, 110.0f);
	m_flFieldOfView = 0.5;
	m_MonsterState = MONSTERSTATE_NONE;
	m_afCapability = bits_CAP_HEAR | bits_CAP_DOORS_GROUP | bits_CAP_TURN_HEAD | bits_CAP_RANGE_ATTACK1;

	m_iRunSequence = LookupSequence("run");
	m_iReloadSequence = LookupSequence("AF_SG_reload");
	m_iShieldSequence = LookupSequence("AF_Tortoise");
	m_iWalkSequence = LookupSequence("walk");
	m_iTurnRightSequence = LookupSequence("turnright");
	m_cAmmoLoaded = 6;
	m_flNextShieldTime = 0;
	m_flNextHealthTime = 0;
	m_healthCycle = FALSE;
	MonsterInit();
	m_distance = 0;
}

void CArmorMan::Precache()
{
	PRECACHE_MODEL("models/armorman/armorman.mdl");
	PRECACHE_SOUND("weapons/dbarrel1.wav");
	PRECACHE_SOUND("weapons/sbarrel1.wav");
	PRECACHE_SOUND("armorman/am_alert1.wav");
	PRECACHE_SOUND("armorman/am_alert2.wav");
	PRECACHE_SOUND("armorman/am_alert3.wav");

	PRECACHE_SOUND("armorman/am_die1.wav");
	PRECACHE_SOUND("armorman/am_die2.wav");

	PRECACHE_SOUND("armorman/am_idle1.wav");
	PRECACHE_SOUND("armorman/am_idle2.wav");
	PRECACHE_SOUND("armorman/am_idle3.wav");

	PRECACHE_SOUND("armorman/am_pain1.wav");
	PRECACHE_SOUND("armorman/am_pain2.wav");

	PRECACHE_SOUND("armorman/am_step1.wav");
	PRECACHE_SOUND("armorman/am_step2.wav");
	PRECACHE_SOUND("armorman/am_step3.wav");
	PRECACHE_SOUND("armorman/am_step4.wav");

	PRECACHE_SOUND("weapons/shotgun_reload_open.wav");
	PRECACHE_SOUND("weapons/shotgun_reload_shell_drop.wav");
	PRECACHE_SOUND("weapons/shotgun_reload_insert.wav");
	PRECACHE_SOUND("weapons/shotgun_reload_close.wav");
}

int CArmorMan::Classify()
{
	return CLASS_HUMAN_MILITARY; // Or whatever classification is appropriate for your game
}

void CArmorMan::SetYawSpeed() {
	pev->yaw_speed = 90;
}

BOOL CArmorMan::CheckAmmo1()
{
	if (m_cAmmoLoaded <= 0){
		SetConditions(bits_COND_NO_AMMO_LOADED);
		return TRUE;
	}
	return FALSE;
}

BOOL CArmorMan::CheckRangeAttack1(float flDot, float flDist)
{

	if (!HasConditions(bits_COND_ENEMY_OCCLUDED) && flDist < 150.0f && flDot >= 0.5)
	{
		TraceResult	tr;
		Vector vecShootOrigin = pev->origin + ARMORMAN_SHOTGUN_OFFSET;


		// verify that a bullet fired from the gun will hit the enemy before the world.
		UTIL_TraceLine(vecShootOrigin, m_hEnemy->BodyTarget(vecShootOrigin), dont_ignore_monsters, ENT(pev), &tr);

		if (tr.flFraction == 1 || tr.pHit == m_hEnemy->edict())
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CArmorMan::CheckWallDistance()
{
	
	TraceResult tr;
	UTIL_TraceLine(pev->origin, pev->origin + gpGlobals->v_forward * 1000, ignore_monsters, ENT(pev), &tr);
	
	if (tr.flFraction <= 0.1f){
		return FALSE;
	}
	return TRUE;
}

void CArmorMan::TurnRight()
{
	pev->angles.y += 90.0f;
}

void CArmorMan::TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType)
{
	
	if (ptr->iHitgroup == 1)
	{
		if (bitsDamageType & (DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB))
		{
			UTIL_Ricochet(ptr->vecEndPos, 1.0f);
			flDamage = 0.01f; 

			// Ensure left arm hitgroup is used to force animation
			ptr->iHitgroup = HITGROUP_LEFTARM;
			
		}
	}

	CBaseMonster::TraceAttack(pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
	
}

void CArmorMan::Shoot(void)
{



	if (m_hEnemy == NULL)
	{
		return;
	}

	int rollValue;
	rollValue = 1 + rand() % 10;
	UTIL_MakeVectors(pev->angles);
	const Vector vecShootOrigin = pev->origin + ARMORMAN_SHOTGUN_OFFSET;
	Vector vecShootDir = ShootAtEnemy(vecShootOrigin);
	//Vector vecShootDir2 = vecShootDir;

	//find the dot product to check if shot is in boundary
	//Vector vecDirToEnemy = (vecShootDir2).Normalize();
	float flDot;

	if (rollValue > 5)
	{
	

			int binaryRand1 = rand() % 2;
			int binaryRand2 = rand() % 2;
			int binaryRand3 = rand() % 2;

		    float rollValue2 = (float)(rand() % 8);
			rollValue2 = rollValue2 / 10;
			float rollValue3 = (float)(rand() % 8);
			rollValue3 = rollValue3 / 10;
			float rollValue4 = (float)(rand() % 8);
			rollValue4 = rollValue4 / 10;

			if (binaryRand1 == 0){
				rollValue2 = rollValue2 * (-1);
			}
			if (binaryRand2 == 0){
				rollValue3 = rollValue3 * (-1);
			}
			if (binaryRand3 == 0){
				rollValue4 = rollValue4 * (-1);
			}
	
			vecShootDir = ShootAtEnemy(vecShootOrigin) + Vector(rollValue2, rollValue3, rollValue4);
			//vecShootDir2 = (ShootAtEnemy(vecShootOrigin) + Vector(rollValue2, rollValue3, rollValue4)).Normalize();

			//flDot = DotProduct(vecShootDir2, gpGlobals->v_forward); // Use gpGlobals->v_forward for NPC's forward direction
	
	}
	const Vector vecAngDir = UTIL_VecToAngles(vecShootDir);

	SetBlending(0, vecAngDir.x);
	pev->effects = EF_MUZZLEFLASH;


	FireBullets(6, vecShootOrigin, vecShootDir, VECTOR_CONE_10DEGREES, 1024.0f, BULLET_PLAYER_BUCKSHOT);
	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "weapons/sbarrel1.wav", VOL_NORM, ATTN_NORM, 0, PITCH_NORM);
	m_cAmmoLoaded--;


	CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, 384, 0.3f);
	return;
}

void CArmorMan::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	switch (pEvent->event)
	{
	case 3:
		Shoot();
		break;
	case 5:
		m_cAmmoLoaded = 6;
		ClearConditions(bits_COND_NO_AMMO_LOADED);
		break;
	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}

}


Schedule_t* CArmorMan::GetSchedule() {

	


	switch (m_MonsterState)
	{
	case MONSTERSTATE_IDLE:
	{
							  if (CheckWallDistance()){
								  
							    return GetScheduleOfType(SCHED_ARMORMAN_WALK);
							  }
							  else{
								  TurnRight();
								  return GetScheduleOfType(SCHED_ARMORMAN_TURN);
							  }



		   break;
	}
	case MONSTERSTATE_COMBAT:
	{

							
								if (HasConditions(bits_COND_SEE_ENEMY)) {

									if (m_hEnemy && pev->health <= (gSkillData.armormanHealth / 3.0f))
									{
										m_healthCycle = TRUE;
									}


									if (m_healthCycle == TRUE && gpGlobals->time > m_flNextShieldTime)
									{
										
										pev->health = gSkillData.armormanHealth / 1.3f;
										m_flNextShieldTime = gpGlobals->time + 30.0f;
										m_healthCycle = FALSE;
										return GetScheduleOfType(SCHED_ARMORMAN_SHIELD);
										
									}

							
									Vector vecEnemyLocation = m_hEnemy->pev->origin - pev->origin;
									float flDist = vecEnemyLocation.Length();
									Vector vecDirToEnemy = (m_hEnemy->pev->origin - pev->origin).Normalize();
									float flDot = DotProduct(vecDirToEnemy, gpGlobals->v_forward); // Use gpGlobals->v_forward for NPC's forward direction

									if (CheckRangeAttack1(flDot, flDist)) {

										return GetScheduleOfType(SCHED_RANGE_ATTACK1);
									}
									else
									{
										return GetScheduleOfType(SCHED_ARMORMAN_CHASE_ENEMY);
									}

								}
								if (CheckAmmo1()){
									if (HasConditions(bits_COND_NO_AMMO_LOADED)) {
										return GetScheduleOfType(SCHED_RELOAD);
									}
								}
							
								
			break;
	}
	default:
	{
			   ALERT(at_aiconsole, "Invalid State for GetSchedule!\n");
			   break;
	}
	}
	return CBaseMonster::GetSchedule();
}

Schedule_t* CArmorMan::GetScheduleOfType(int Type)
{
	switch (Type)
	{
	case SCHED_ARMORMAN_CHASE_ENEMY:
		return &slArmorManChase[0];
		break;
	case SCHED_RELOAD:
		return &slArmorManReload[0];
		break;
	case SCHED_ARMORMAN_SHIELD:
		return &slArmorManShield[0];
		break;
	case SCHED_ARMORMAN_WALK:
		return &slArmorManWalk[0];
	case SCHED_ARMORMAN_TURN:
		return &slArmorManTurn[0];
	default:
		ALERT(at_aiconsole, "Invalid State for GetScheduleOfType!\n");
	}
	return CBaseMonster::GetScheduleOfType(Type);
}





void CArmorMan::RunTask(Task_t* pTask)
{
	ALERT(at_console, "Current position: %f, %f, %f\n", pev->origin.x, pev->origin.y, pev->origin.z);
	ALERT(at_console, "Goal position: %d\n", m_distance);

	switch (pTask->iTask)
	{

	case TASK_GET_PATH_TO_LOCATION:
		if (FRouteClear()) // Check if the path is complete
		{
			
			TaskComplete();
		}
		else
		{
			
		}
		break;

	case TASK_ARMORMAN_TURN:

		if (m_iTurnRightSequence != -1 && pev->sequence != m_iTurnRightSequence)
		{
			pev->sequence = m_iTurnRightSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;

	case TASK_WALK_PATH:

		if (m_iWalkSequence != -1 && pev->sequence != m_iWalkSequence)
		{
		
			pev->sequence = m_iWalkSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
	
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;
	case TASK_RUN_PATH:

		if (m_iRunSequence != -1 && pev->sequence != m_iRunSequence)
		{
			pev->sequence = m_iRunSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;
	case TASK_RELOAD:

		if (m_iReloadSequence != -1 && pev->sequence != m_iReloadSequence)
		{
			pev->sequence = m_iReloadSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;
	case TASK_ARMORMAN_SHIELD:

		if (m_iShieldSequence != -1 && pev->sequence != m_iShieldSequence)
		{
			pev->sequence = m_iShieldSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		// Run the path as usual
		CBaseMonster::RunTask(pTask);
		break;
	default:
		CBaseMonster::RunTask(pTask);
		break;
	}
}

void CArmorMan::StartTask(Task_t* pTask)
{
	
	UTIL_MakeVectors(pev->angles);
	TraceResult tr;
	UTIL_TraceLine(pev->origin, pev->origin + gpGlobals->v_forward * 1000, ignore_monsters, ENT(pev), &tr);
	m_distance = (int)(tr.flFraction * 1000);
	Vector vecGoal = pev->origin + gpGlobals->v_forward * (m_distance * (0.70f)); // Forward by a certain distance
	
	switch (pTask->iTask)
	{
	case TASK_GET_PATH_TO_LOCATION:

		if (BuildRoute(vecGoal, bits_MF_TO_LOCATION, NULL))
		{
			TaskComplete();
		}
		else
		{
			TaskFail();
		}
		break;
		
		
	case TASK_ARMORMAN_TURN:
		if (m_iTurnRightSequence != -1)
		{
			pev->sequence = m_iTurnRightSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		TaskComplete();
		break;
	case TASK_ARMORMAN_WALK:
		if (m_iWalkSequence != -1)
		{
			pev->sequence = m_iWalkSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		
		TaskComplete();
		break;
	case TASK_ARMORMAN_CHASE_ENEMY:
		// Start the running animation
	
		if (m_iRunSequence != -1)
		{
			pev->sequence = m_iRunSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		TaskComplete();
		break;
	case TASK_RELOAD:
		

		if (m_iReloadSequence != -1)
		{
			pev->sequence = m_iReloadSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		TaskComplete();
		break;
	case TASK_ARMORMAN_SHIELD:


		if (m_iShieldSequence != -1)
		{
			pev->sequence = m_iShieldSequence;
			pev->frame = 0;
			ResetSequenceInfo();
		}
		TaskComplete();
		break;
	default:
		CBaseMonster::StartTask(pTask);
		break;
	}
}

