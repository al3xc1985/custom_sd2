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
SDName: boss_marwyn
SD%Complete: 30%
SDComment:
SDCategory: Halls of Reflection
EndScriptData */

#include "precompiled.h"
#include "halls_of_reflection.h"

enum
{
    SAY_INTRO1              = -1610109,
    SAY_INTRO2              = -1610110,
    SAY_AGGRO               = -1610111,
    SAY_CORRUPTED_FLESH1    = -1610112,
    SAY_CORRUPTED_FLESH2    = -1610113,
    SAY_SLAY1               = -1610114,
    SAY_SLAY2               = -1610115,
    SAY_DEATH               = -1610116,

    SPELL_BERSERK                           = 47008,
    SPELL_OBLITERATE                        = 72360,
    SPELL_OBLITERATE_H                      = 72434,
    SPELL_SHARED_SUFFERING                  = 72368,
    SPELL_SHARED_SUFFERING_H                = 72369,
    SPELL_WELL_OF_CORRUPTION                = 72362,
    SPELL_CORRUPTED_FLESH                   = 72363,
    SPELL_CORRUPTED_FLESH_H                 = 72436,

    EQUIP_ID                                = 50672,

};

struct MANGOS_DLL_DECL boss_marwynAI : public ScriptedAI
{
    boss_marwynAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        pCreature->setFaction(35);
        pCreature->SetVisibility(VISIBILITY_OFF);
        SetEquipmentSlots(false, EQUIP_ID, -1, -1);
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiSharedSuffering_Timer;
    uint32 m_uiWell_Timer;
    uint32 m_uiTouch_Timer;
    uint32 m_uiFlesh_Timer;
    uint32 m_uiObliterate_Timer;
    uint32 m_uiSummon_Timer;

    uint8 SummonCount;
    bool hasIntro;

    uint64 npctype1;
    uint64 npctype2;
    uint64 npctype3;
    uint64 npctype4;

    void Reset()
    {
        m_uiBerserk_Timer       = 180000;
        m_uiSharedSuffering_Timer = 4000;
        m_uiWell_Timer          = 5000;
        m_uiTouch_Timer         = 8000;
        m_uiFlesh_Timer         = 10000;
        m_uiObliterate_Timer    = 1000;
        SummonCount             = 0;
        m_uiSummon_Timer        = 60000;
        hasIntro                = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARWYN, NOT_STARTED);

        ResetEvent();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
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
            if(Creature *pJaina = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_JAINA_INTRO))))
            {
                pJaina->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pJaina->SetVisibility(VISIBILITY_ON);
                pJaina->GetMap()->CreatureRelocation(pJaina, 5266.78f, 1953.42f, 707.697f, 0.740877f);
                pJaina->SendMonsterMove(5266.78f, 1953.42f, 707.697f, SPLINETYPE_NORMAL, pJaina->GetSplineFlags(), 1);
            }
        }
        else if(TeamInInstance == HORDE) 
        {
            if(Creature *pSylvanas = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_SYLVANAS_INTRO))))
            {
                pSylvanas->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                pSylvanas->SetVisibility(VISIBILITY_ON);
                pSylvanas->GetMap()->CreatureRelocation(pSylvanas, 5266.78f, 1953.42f, 707.697f, 0.740877f);
                pSylvanas->SendMonsterMove(5266.78f, 1953.42f, 707.697f, SPLINETYPE_NORMAL, pSylvanas->GetSplineFlags(), 1);
            }
        }
    }

    bool CallGuards(TempSummonType type, uint32 _summontime )
    {
        switch(urand(0,3))
        {
        case 0: {
            npctype1 = MOB_SHADOWY_MERCENARY;
            npctype2 = MOB_SPECTRAL_FOOTMAN;
            npctype3 = MOB_GHOSTLY_PRIEST;
            npctype4 = MOB_PHANTOM_HALLUCINATION;
            break;}
        case 1: {
            npctype1 = MOB_TORTURED_RIFLEMAN;
            npctype2 = MOB_SPECTRAL_FOOTMAN;
            npctype3 = MOB_PHANTOM_MAGE;
            npctype4 = MOB_GHOSTLY_PRIEST;
            break;}
        case 2: {
            npctype1 = MOB_TORTURED_RIFLEMAN;
            npctype2 = MOB_PHANTOM_HALLUCINATION;
            npctype3 = MOB_GHOSTLY_PRIEST;
            npctype4 = MOB_SHADOWY_MERCENARY;
            break;}
        case 3: {
            npctype1 = MOB_SHADOWY_MERCENARY;
            npctype2 = MOB_PHANTOM_HALLUCINATION;
            npctype3 = MOB_PHANTOM_MAGE;
            npctype4 = MOB_SPECTRAL_FOOTMAN;
            break;}
        }
        if (Creature* pSummon1 = m_creature->SummonCreature(npctype1, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, type, _summontime))
            pSummon1->SetInCombatWithZone();
        if (Creature* pSummon2 = m_creature->SummonCreature(npctype2, SpawnLoc[1].x, SpawnLoc[1].y, SpawnLoc[1].z, 0, type, _summontime))
            pSummon2->SetInCombatWithZone();
        if (Creature* pSummon3 = m_creature->SummonCreature(npctype3, SpawnLoc[2].x, SpawnLoc[2].y, SpawnLoc[2].z, 0, type, _summontime))
            pSummon3->SetInCombatWithZone();
        if (Creature* pSummon4 = m_creature->SummonCreature(npctype4, SpawnLoc[3].x, SpawnLoc[3].y, SpawnLoc[3].z, 0, type, _summontime))
            pSummon4->SetInCombatWithZone();

        return true;
    }

    void AttackStart(Unit* pWho)
    {
        if (!m_pInstance)  
            return;

        if (m_pInstance->GetData(TYPE_MARWYN) != IN_PROGRESS) 
            return;

        if (m_creature->Attack(pWho, true)) 
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho);
        }
    }

    void Aggro(Unit *who) 
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_MARWYN, IN_PROGRESS);
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 1))
        {
        case 0: DoScriptText(SAY_SLAY1, m_creature); break;
        case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit *killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_MARWYN, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance->GetData(TYPE_MARWYN) == SPECIAL ) 
        {
            if (m_uiSummon_Timer < uiDiff) 
            {
                if(!hasIntro)
                {
                    DoScriptText(SAY_INTRO1, m_creature);
                    hasIntro = true;
                }

                ++SummonCount;
                if (SummonCount > MOB_WAVES_NUM_1) 
                {
                    m_pInstance->SetData(TYPE_MARWYN, IN_PROGRESS);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->SetInCombatWithZone();
                }
                else 
                    CallGuards(TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000);
                m_uiSummon_Timer = MOB_WAVES_DELAY_1;
            } 
            else 
                m_uiSummon_Timer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if(m_pInstance && m_pInstance->GetData(TYPE_MARWYN) == IN_PROGRESS)
                Reset();
            return;
        }

        if (m_uiSharedSuffering_Timer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_SHARED_SUFFERING : SPELL_SHARED_SUFFERING_H);
            m_uiSharedSuffering_Timer= 20000;
        } 
        else 
            m_uiSharedSuffering_Timer -= uiDiff;

        if (m_uiWell_Timer < uiDiff) 
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WELL_OF_CORRUPTION);
            m_uiWell_Timer= 30000;
        } 
        else 
            m_uiWell_Timer -= uiDiff;

        /*if (m_uiTouch_Timer < uiDiff) 
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_WELL_OF_CORRUPTION);
            m_uiTouch_Timer= 30000;
        } 
        else 
            m_uiTouch_Timer -= uiDiff;*/

        if (m_uiFlesh_Timer < uiDiff) 
        {
            switch(urand(0, 1))
            {
            case 0: DoScriptText(SAY_CORRUPTED_FLESH1, m_creature); break;
            case 1: DoScriptText(SAY_CORRUPTED_FLESH2, m_creature); break;
            }
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, m_bIsRegularMode ? SPELL_CORRUPTED_FLESH : SPELL_CORRUPTED_FLESH_H);
            m_uiFlesh_Timer= 10000;
        } 
        else 
            m_uiFlesh_Timer -= uiDiff;

        if (m_uiObliterate_Timer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OBLITERATE : SPELL_OBLITERATE_H);
            m_uiObliterate_Timer=urand(8000,12000);
        } 
        else 
            m_uiObliterate_Timer -= uiDiff;

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

CreatureAI* GetAI_boss_marwyn(Creature* pCreature)
{
    return new boss_marwynAI(pCreature);
}

void AddSC_boss_marwyn()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_marwyn";
    newscript->GetAI = &GetAI_boss_marwyn;
    newscript->RegisterSelf();
}