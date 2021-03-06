/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: boss_falryn
SD%Complete: 40%
SDComment:
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "halls_of_reflection.h"

enum
{
    SAY_INTRO1              = -1610101,
    SAY_INTRO2              = -1610102,
    SAY_AGGRO               = -1610103,
    SAY_IMPENDING_DESPAIR   = -1610104,
    SAY_DEFILING_HORROR     = -1610105,
    SAY_SLAY1               = -1610106,
    SAY_SLAY2               = -1610107,
    SAY_DEATH               = -1610108,

    //common
    SPELL_BERSERK                           = 47008,
    SPELL_HOPELESSNESS                      = 72395,
    SPELL_IMPENDING_DESPAIR                 = 72426,
    SPELL_DEFILING_HORROR                   = 72435,
    SPELL_DEFILING_HORROR_H                 = 72452,
    SPELL_QUIVERING_STRIKE                  = 72422,
    SPELL_QUIVERING_STRIKE_H                = 72453,

    EQUIP_ID                                = 50704,
};

struct MANGOS_DLL_DECL boss_falricAI : public ScriptedAI
{
    boss_falricAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBerserk_Timer;
    uint32 m_uiDespair_Timer;
    uint32 m_uiHorror_Timer;
    uint32 m_uiStrike_Timer;
    uint32 m_uiSummon_Timer;
    uint32 m_uiExploitCheckTimer;

    bool m_bIsPhase1;
    bool m_bIsPhase2;
    bool m_bIsPhase3;

    uint8 m_uiSummonCount;
    bool m_bHasIntro;

	uint8 m_uiCalledSoldiers;
	uint8 m_uiDeadSoldiers;
	uint8 m_uiMaxMobs;
	bool m_bDelaySet;

	std::list<uint64> m_lSoldiersGUIDList;
	uint32 m_uiLocNo;
	bool m_bIsCall;

    void Reset()
    {
        m_uiBerserk_Timer       = 180000;
        m_uiSummonCount         = 0;
        m_uiExploitCheckTimer   = 1000;
        m_bHasIntro             = false;
        m_bIsPhase1             = false;
        m_bIsPhase2             = false;
        m_bIsPhase3             = false;

		m_uiCalledSoldiers		= 0;
		m_uiDeadSoldiers		= 0;
		m_uiMaxMobs				= 3;
		m_bDelaySet				= true;
        
        m_uiDespair_Timer       = m_bIsRegularMode ? 40000 : 30000;
        m_uiHorror_Timer        = urand(25000,35000);
        m_uiStrike_Timer        = urand(10000,15000);
        m_uiSummon_Timer        = 1000;

		m_bIsCall				= false;
		m_lSoldiersGUIDList.clear();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
		m_creature->SetRespawnDelay(DAY);
    }

    void JustReachedHome()
    {
        if (m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FALRIC, NOT_STARTED);
            m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES, 0);
        }

        ResetEvent();
    }

    void ResetEvent()
    {
        uint32 TeamInInstance = 0;

        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                if (Player* pPlayer = PlayerList.begin()->getSource())
                    TeamInInstance = pPlayer->GetTeam();
            }
        }
        if(TeamInInstance == ALLIANCE)
        {
            if(Creature *pJaina = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_JAINA_INTRO)))
            {
                pJaina->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pJaina->SetVisibility(VISIBILITY_ON);
                pJaina->GetMap()->CreatureRelocation(pJaina, 5266.78f, 1953.42f, 707.697f, 0.740877f);
                pJaina->SendMonsterMove(5266.78f, 1953.42f, 707.697f, SPLINETYPE_NORMAL, pJaina->GetSplineFlags(), 1);
            }
        }
        else if(TeamInInstance == HORDE) 
        {
            if(Creature *pSylvanas = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(DATA_SYLVANAS_INTRO)))
            {
                pSylvanas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pSylvanas->SetVisibility(VISIBILITY_ON);
                pSylvanas->GetMap()->CreatureRelocation(pSylvanas, 5266.78f, 1953.42f, 707.697f, 0.740877f);
                pSylvanas->SendMonsterMove(5266.78f, 1953.42f, 707.697f, SPLINETYPE_NORMAL, pSylvanas->GetSplineFlags(), 1);
            }
        }
		if(Creature* pMarwyn = GetClosestCreatureWithEntry(m_creature, NPC_MARWYN, 100))
			pMarwyn->ForcedDespawn();
		m_creature->ForcedDespawn();
    }

	void Summon()
	{
		uint32 m_uiSummonId;
		m_uiLocNo = 0;

		for(uint8 i = 0; i < 18; ++i)
		{
			switch(urand(0,3))
			{
			case 0:
				switch(urand(1, 3))
				{
				case 1: m_uiSummonId = MOB_SHADOWY_MERCENARY;	break;
				case 2: m_uiSummonId = MOB_SPECTRAL_FOOTMAN;	break;
				case 3: m_uiSummonId = MOB_GHOSTLY_PRIEST;		break;
				}
				break;
			case 1: 
				switch(urand(1, 3))
				{
				case 1: m_uiSummonId = MOB_TORTURED_RIFLEMAN;	break;
				case 2: m_uiSummonId = MOB_SPECTRAL_FOOTMAN;	break;
				case 3: m_uiSummonId = MOB_PHANTOM_MAGE;		break;
				}
				break;
			case 2: 
				switch(urand(1, 3))
				{
				case 1: m_uiSummonId = MOB_TORTURED_RIFLEMAN;		break;
				case 2: m_uiSummonId = MOB_SPECTRAL_FOOTMAN;		break;
				case 3: m_uiSummonId = MOB_GHOSTLY_PRIEST;			break;
				}
				break;
			case 3: 
				switch(urand(1, 3))
				{
				case 1: m_uiSummonId = MOB_SHADOWY_MERCENARY;		break;
				case 2: m_uiSummonId = MOB_TORTURED_RIFLEMAN;		break;
				case 3: m_uiSummonId = MOB_PHANTOM_MAGE;			break;
				}
				break;
			}

			if(Creature* pSummon = m_creature->SummonCreature(m_uiSummonId, SpawnLoc[m_uiLocNo].x, SpawnLoc[m_uiLocNo].y, SpawnLoc[m_uiLocNo].z, SpawnLoc[m_uiLocNo].o, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
				m_lSoldiersGUIDList.push_back(pSummon->GetGUID());
			m_uiLocNo += 1;
		}
	}

	void CallFallSoldier()
	{
		for(uint8 i = 0; i < m_uiMaxMobs; ++i)
		{
			if(!m_lSoldiersGUIDList.empty())
			{
				std::list<uint64>::iterator iter = m_lSoldiersGUIDList.begin();
				// no random
				//advance(iter, urand(0, m_lSoldiersGUIDList.size()-1));

				if (Creature* pTemp = m_creature->GetMap()->GetCreature(*iter))
				{
					if(pTemp->isAlive())
					{
						m_uiCalledSoldiers += 1;
						pTemp->setFaction(14);
						pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
						pTemp->SetInCombatWithZone();
						m_lSoldiersGUIDList.remove(*iter);
					}
				}
			}
		}
	}

	void SummonedCreatureJustDied(Creature* pSummon)
	{
		m_uiDeadSoldiers += 1;
	}

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);  
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_FALRIC) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_FALRIC, DONE);
            m_pInstance->SetData(TYPE_MARWYN, SPECIAL);
			// save boss as done if event resets
			if(m_pInstance->GetData(TYPE_SPIRIT_EVENT) == DONE)
				m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
			else
				m_pInstance->SetData(TYPE_SPIRIT_EVENT, DONE);
        }
    }

    bool IsPlayerInside()
    {
        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();

            if (PlayerList.isEmpty())
                return false;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                if(GameObject* pAltar = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_ALTAR)))
                {
                    if (i->getSource()->isAlive() && i->getSource()->GetDistance2d(pAltar) < 75.0f)
                        return true;
                }
            }
        } 

        return false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_INTRO) == DONE && m_pInstance->GetData(TYPE_FALRIC) == SPECIAL)
        {
            if(m_uiExploitCheckTimer < uiDiff)
            {
                if(!IsPlayerInside())
                    EnterEvadeMode();
                m_uiExploitCheckTimer = 1000;
            }
            else m_uiExploitCheckTimer -= uiDiff;

			if(!m_bIsCall) 
            {
               m_bIsCall = true;
               Summon();
            }

			if(m_uiDeadSoldiers == m_uiCalledSoldiers && !m_bDelaySet && m_uiSummonCount < 5)
			{
				m_uiSummon_Timer = 10000;
				m_bDelaySet = true;
			}

            if (m_uiSummon_Timer < uiDiff) 
            {
                if(!m_bHasIntro)
                {
                    if(m_pInstance)
                        m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES, 1);
                    DoScriptText(SAY_INTRO1, m_creature);
                    m_bHasIntro = true;
                    m_creature->SetInCombatWithZone();
                }

                m_uiSummonCount += 1;
                if (m_uiSummonCount > 4)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetInCombatWithZone();
                    if(m_pInstance) 
					{
                        m_pInstance->SetData(TYPE_FALRIC, IN_PROGRESS);
						m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES_COUNT, m_uiSummonCount);
					}
                }
                else 
                {
                    if(m_pInstance)
                        m_pInstance->DoUpdateWorldState(UI_STATE_SPIRIT_WAVES_COUNT, m_uiSummonCount);
					// change the no. of mobs per wave
					switch(m_uiSummonCount)
					{
					case 1:
						m_uiMaxMobs = 5;	// maybe just 3?
						break;
					case 2:
					case 3:
						m_uiMaxMobs = 4;
						break;
					case 4:
						m_uiMaxMobs = 5;
						break;
					}
					CallFallSoldier();
                }
				m_bDelaySet = false;
                m_uiSummon_Timer = 90000;
            } 
            else 
                m_uiSummon_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(m_pInstance && m_pInstance->GetData(TYPE_FALRIC) == IN_PROGRESS)
                EnterEvadeMode();
            return;
        }

        if (m_uiDespair_Timer < uiDiff) 
        {
            DoScriptText(SAY_IMPENDING_DESPAIR, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_IMPENDING_DESPAIR);
            m_uiDespair_Timer= m_bIsRegularMode ? 40000 : 30000;
        } 
        else 
            m_uiDespair_Timer -= uiDiff;

        if (m_uiStrike_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_QUIVERING_STRIKE : SPELL_QUIVERING_STRIKE_H);
            m_uiStrike_Timer=urand(10000,15000);
        } 
        else 
            m_uiStrike_Timer -= uiDiff;

        if (m_uiHorror_Timer < uiDiff) 
        {
            DoScriptText(SAY_DEFILING_HORROR, m_creature);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_DEFILING_HORROR : SPELL_DEFILING_HORROR_H);
            m_uiHorror_Timer=urand(25000,35000);
        } 
        else 
            m_uiHorror_Timer -= uiDiff;

        if(m_creature->GetHealthPercent() <= 66.0f && !m_bIsPhase1)
        {
            DoCast(m_creature, SPELL_HOPELESSNESS);
            m_bIsPhase1 = true;
        }

        if(m_creature->GetHealthPercent() <= 33.0f && !m_bIsPhase2)
        {
            DoCast(m_creature, SPELL_HOPELESSNESS);
            m_bIsPhase2 = true;
        }

        if(m_creature->GetHealthPercent() <= 10.0f && !m_bIsPhase3)
        {
            DoCast(m_creature, SPELL_HOPELESSNESS);
            m_bIsPhase3 = true;
        }

        if (m_uiBerserk_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserk_Timer = 180000;
        } 
        else  
            m_uiBerserk_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_falric(Creature* pCreature)
{
    return new boss_falricAI(pCreature);
}

void AddSC_boss_falric()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_falric";
    newscript->GetAI = &GetAI_boss_falric;
    newscript->RegisterSelf();
}
