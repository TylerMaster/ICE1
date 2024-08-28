/***************************
Where Is Poppy
2024-05-21

wip_monster.cpp

******************************/

#include "wip_monster.h"

//#define TASK_DRONE_CHASE_ENEMY (LAST_COMMON_TASK + 1)
//#define SCHED_DRONE_CHASE_ENEMY (LAST_COMMON_SCHEDULE + 1)


//#define SCHED_DRONE_RELOAD (LAST_COMMON_SCHEDULE + 2)

Task_t tlDroneChase[] = {
	{ TASK_GET_PATH_TO_ENEMY, 0.0f },
	{ TASK_RUN_PATH, 0.0f },
	{ TASK_WAIT, 0.0f },
};

Schedule_t slDroneChase[] = {
	{
		tlDroneChase,
		ARRAYSIZE(tlDroneChase),
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_RANGE_ATTACK1,
		0,
		"Drone Chase"
	}
};


class CPitDroneSpike : public CBaseEntity
{
public:
	void Spawn(void);
	void Touch(CBaseEntity *pOther);
	Vector waterSpeed;

};

LINK_ENTITY_TO_CLASS(pitdrone_spike, CPitDroneSpike);

void CPitDroneSpike::Spawn(void)
{
	PRECACHE_MODEL("models/npcs/pit_drone_spike.mdl");

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_FLY;
	pev->classname = MAKE_STRING("pitdrone_spike");

	SET_MODEL(ENT(pev), "models/npcs/pit_drone_spike.mdl");
}

void CPitDroneSpike::Touch(CBaseEntity *pOther)
{
	if (!pOther->pev->takedamage) {
		// If the entity doesn't take damage
		if (UTIL_PointContents(pev->origin) == CONTENTS_WATER)
		{
			pev->velocity = waterSpeed; // Go slower while in water
		}
		else {
			pev->solid = SOLID_NOT;
			pev->movetype = MOVETYPE_FLY;
			pev->velocity = Vector(0, 0, 0);
			UTIL_Sparks(pev->origin);
			EMIT_SOUND(ENT(pev), CHAN_VOICE, "pitdrone/pit_drone_eat.wav", 1, ATTN_NORM);
		}
	}
	else {
		
		// If it does take damage
		pOther->TakeDamage(pev, pev, 15, DMG_GENERIC); // Give damage to whatever it is
		UTIL_Remove(this); // Remove it
	}
}



class CPitDrone : public CBaseMonster
{
public:
	void Spawn(void);
	void Precache(void);

	int Classify(void);
	void SetYawSpeed(void);
	void HandleAnimEvent(MonsterEvent_t *pEvent) override;
	Schedule_t* GetScheduleOfType(int Type) override;
	Schedule_t *GetSchedule(void) override; // Handles some schedules
	void RunTask(Task_t* pTask) override;
	void StartTask(Task_t* pTask) override;
	BOOL CheckRangeAttack1(float flDot, float flDist) override;

	BOOL CPitDrone::CheckAmmo1();

	CUSTOM_SCHEDULES;

private:
	int m_iRunSequence;
	
	int m_iSpikes;

};

LINK_ENTITY_TO_CLASS(wip_MonsterPitDrone, CPitDrone);

DEFINE_CUSTOM_SCHEDULES(CPitDrone)
{
	slDroneChase,
		
};

IMPLEMENT_CUSTOM_SCHEDULES(CPitDrone, CBaseMonster);

void CPitDrone::Spawn()
{
	Precache();

	SET_MODEL(ENT(pev), "models/pit_drone.mdl");


	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX); // Let's make his size human. If you're smart enough (or have lots of patience) you can get replace the VEC_ stuff with "Vector( x, y, z)".

	pev->solid = SOLID_SLIDEBOX; // They see me slidin', they hating. This actually tells the engine for it to be Solid. Snakes can GTHO.
	pev->movetype = MOVETYPE_FLY; // 'cause monsters walk - they don't drive (Nightmares will follow)
	m_bloodColor = BLOOD_COLOR_RED; // Green blood - just like this comment. Freaked out much? (The blood's actually yellow, though)
	pev->health = 100; // Health - let's keep it as an integer, as opposed to a changeable variable for now.
	pev->view_ofs = Vector(0, 0, 20); // Eyes' offset (He sees you doing stuff you shouldn't)
	m_flFieldOfView = 0.5; // How far he can see.
	m_MonsterState = MONSTERSTATE_NONE; // Afet he spawns, make him sit there like an idiot, doing nothing.

	m_iRunSequence = LookupSequence("run");
	//m_iReloadSequence = LookupSequence("reload");

	MonsterInit(); // Starts the monsters AI

	m_iSpikes = 6; // Default, he's fully loaded with spikes. AGAIN, NO PUNS YOU SICKOS!
}

int CPitDrone::Classify(void)
{
	return CLASS_ALIEN_MONSTER;
}

void CPitDrone::SetYawSpeed(void) {
	pev->yaw_speed = 90;
}

BOOL CPitDrone::CheckAmmo1()
{
	if (m_iSpikes <= 0){
		SetConditions(bits_COND_NO_AMMO_LOADED);
		return TRUE;
	}
	return FALSE;
}



void CPitDrone::Precache()
{
	PRECACHE_MODEL("models/npcs/pit_drone_spike.mdl"); //Loads the model for the spike
	PRECACHE_MODEL("models/pit_drone.mdl"); //Loads the NPC model in the game

	// Bunch of pretty self-explanatory sounds
	PRECACHE_SOUND("pitdrone/pit_drone_melee_attack1.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_melee_attack2.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_attack_spike1.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_eat.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_die1.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_die2.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_die3.wav");
	PRECACHE_SOUND("pitdrone/pit_drone_hunt3.wav");
}


BOOL CPitDrone::CheckRangeAttack1(float flDot, float flDist)
{

	if (!HasConditions(bits_COND_ENEMY_OCCLUDED))
	{
		TraceResult	tr;
		Vector vecspikeOffset;
		Vector vecspikeDir;


		UTIL_MakeVectors(pev->angles);


		vecspikeOffset = (gpGlobals->v_forward * 22 + gpGlobals->v_up * 40);


		vecspikeOffset = (pev->origin + vecspikeOffset);

		// verify that a bullet fired from the gun will hit the enemy before the world.
		UTIL_TraceLine(vecspikeOffset, m_hEnemy->BodyTarget(vecspikeOffset), dont_ignore_monsters, ENT(pev), &tr);

		if (tr.flFraction == 1 || tr.pHit == m_hEnemy->edict())
		{
			return TRUE;
		}
	}
	return FALSE;
}


void CPitDrone::HandleAnimEvent(MonsterEvent_t* pEvent)
{
	switch (pEvent->event)
	{
		{
	case PDRONE_RELOAD:
		m_iSpikes = 6;
		SetBodygroup(BODYGROUP_SPIKES, 1);
		break;
		}
	case PDRONE_FIRE_SPIKE:
	{
							  Vector vecspikeOffset;
							  Vector vecspikeDir;


							  UTIL_MakeVectors(pev->angles);


							  vecspikeOffset = (gpGlobals->v_forward * 22 + gpGlobals->v_up * 40);


							  vecspikeOffset = (pev->origin + vecspikeOffset);


							  vecspikeDir = ((m_hEnemy->pev->origin + m_hEnemy->pev->view_ofs) - vecspikeOffset).Normalize();


							  vecspikeDir.x += RANDOM_FLOAT(-0.01, 0.01);
							  vecspikeDir.y += RANDOM_FLOAT(-0.01, 0.01);
							  vecspikeDir.z += RANDOM_FLOAT(-0.01, 0.01);


							  CPitDroneSpike *pSpike = (CPitDroneSpike *)CBaseMonster::Create("pitdrone_spike", vecspikeOffset, pev->angles, edict());

							  pSpike->pev->velocity = vecspikeDir * 900;

							  pSpike->waterSpeed = vecspikeDir * 300;


							  pSpike->pev->owner = ENT(pev);


							  pSpike->pev->friction = 0;

							  // Set the angles to correspond to the velocity
							  pSpike->pev->angles = UTIL_VecToAngles(pSpike->pev->velocity);

							  // Take a spike out
							  m_iSpikes--;

							  // Set the body to match the spikes.
							  if (m_iSpikes == 0)
							  {
								  SetBodygroup(BODYGROUP_SPIKES, 0);
							  }
							  else
							  {
								  SetBodygroup(BODYGROUP_SPIKES, GetBodygroup(BODYGROUP_SPIKES) + 1);
							  }
							  break;
	}
	case PDRONE_MELEE_LEFT:
	{
							  // Only gonna comment on this one, cuz the rest are basically the same.
							  // This gets the enemy and attacks at the same time.
							  // The parameters after CheckTraceHullAttack are distance, amount of damage, and type
							  CBaseEntity *pHurt = CheckTraceHullAttack(85, 20, DMG_SLASH);

							  // If you did hurt someone...
							  if (pHurt)
							  {
								  // ...make him change his view angle a bit (only players)...
								  pHurt->pev->punchangle.y = 15;
								  pHurt->pev->punchangle.x = 8;
								  // ... and push him back a bit.
								  pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_up * -100;
							  }
							  break;
	}
	case PDRONE_MELEE_RIGHT:
	{
							   CBaseEntity *pHurt = CheckTraceHullAttack(85, 20, DMG_SLASH);

							   if (pHurt)
							   {
								   pHurt->pev->punchangle.y = -15;
								   pHurt->pev->punchangle.x = 8;
								   pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_up * -100;
							   }
							   break;
	}
	case PDRONE_MELEE_BOTH:
	{
							  CBaseEntity *pHurt = CheckTraceHullAttack(85, 30, DMG_SLASH);

							  if (pHurt)
							  {
								  pHurt->pev->punchangle.x = 15;
								  pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_up * -100;
							  }
							  break;
	}
	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}

}

Schedule_t* CPitDrone::GetSchedule(void)
{



	switch (m_MonsterState)
	{
		// Manly monster needs to fight
	case MONSTERSTATE_COMBAT:
		if (HasConditions(bits_COND_ENEMY_DEAD))
		{
			// The enemy is dead - call base class, all code to handle dead enemies
			// is
			// centralized there.
			return CBaseMonster::GetSchedule();
		}

		// Can I attack melee style?
		if (HasConditions(bits_COND_CAN_MELEE_ATTACK1))
		{
			// Randomize my melee attacks, so it's a bit different
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


		if (CheckAmmo1()){
			if (HasConditions(bits_COND_NO_AMMO_LOADED)) {
				return GetScheduleOfType(SCHED_RELOAD);
			}
		}

		Vector vecEnemyLocation = m_hEnemy->pev->origin - pev->origin;
		float flDist = vecEnemyLocation.Length();
		Vector vecDirToEnemy = (m_hEnemy->pev->origin - pev->origin).Normalize();
		float flDot = DotProduct(vecDirToEnemy, gpGlobals->v_forward); // Use gpGlobals->v_forward for NPC's forward direction
		BOOL range;
		range = CheckRangeAttack1(flDot, flDist);
		ALERT(at_console, "Range: %d\n", range);
		
		
		// I can range attack! HELLZ YEAH!
		if (CheckRangeAttack1(flDot, flDist))
		{
			// TOO CLOSE! USE MELEE.
			if ((pev->origin - m_hEnemy->pev->origin).Length() <= 256)
			{
				return GetScheduleOfType(SCHED_CHASE_ENEMY);
			}
			if ((pev->origin - m_hEnemy->pev->origin).Length() <= 512)
			{
				// Do I have spikes?
				if (CheckRangeAttack1(flDot, flDist))
				{
					// Yes. Fire!
					return GetScheduleOfType(SCHED_RANGE_ATTACK1);
				}
				else
				{
					// No.
					if ((pev->origin - m_hEnemy->pev->origin).Length() <= 312)
					{
						// I'm close, I can go and attack.
						if (HasConditions(bits_COND_CAN_MELEE_ATTACK1))
						{
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
						// Lemme get a bit closer, so I can attack
						else
						{
							return GetScheduleOfType(SCHED_CHASE_ENEMY);
						}
					}
					// He's too far to melee. I'll just reload
					else
					{
						return GetScheduleOfType(SCHED_RELOAD);
					}
				}
			}
			// Too far to either fire the spikes or melee, so lemme just get closer,
			// so I'm more accurate.
			else
			{
				return GetScheduleOfType(SCHED_CHASE_ENEMY);
			}
		}
		// If I can do nothing, just chase after him
		return GetScheduleOfType(SCHED_CHASE_ENEMY);
		break;
	}

	// The base probably knows what to do
	return CBaseMonster::GetSchedule();
}


Schedule_t* CPitDrone::GetScheduleOfType(int Type)
{
	switch (Type)
	{
	case SCHED_CHASE_ENEMY:
		return &slDroneChase[0];

	default:
		ALERT(at_console, "GetScheduleOfType()nNo CASE for Schedule Type %d!n", Type);
		break;
	}

	return CBaseMonster::GetScheduleOfType(Type);
}

void CPitDrone::RunTask(Task_t* pTask)
{


	switch (pTask->iTask)
	{

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

	default:
		CBaseMonster::RunTask(pTask);
		break;
	}
}

void CPitDrone::StartTask(Task_t* pTask)
{
	
	ALERT(at_console, "Spike COUNT: %d\n", m_iSpikes);
	
	switch (pTask->iTask)
	{
	
	default:
		CBaseMonster::StartTask(pTask);
		break;
	}
}


