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
SDName: Boss_Loatheb
SD%Complete: 100
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_AURA_BLOCKING     = -1533143,
    EMOTE_AURA_WANE         = -1533144,
    EMOTE_AURA_FADING       = -1533145,

    SPELL_DEATHBLOOM        = 29865,
    SPELL_DEATHBLOOM_H      = 55053,
    SPELL_INEVITABLE_DOOM   = 29204,
    SPELL_INEVITABLE_DOOM_H = 55052,
    SPELL_NECROTIC_AURA     = 55593,
    SPELL_SUMMON_SPORE      = 29234,
    SPELL_BERSERK           = 26662,

    SPELL_FUNGAL_CREEP      = 29232,
    NPC_SPORE               = 16286,

    ACHIEV_SPORE_LOSER      = 2182,
    ACHIEV_SPORE_LOSER_H    = 2183,
};

#define ADD_1X 2957.040f
#define ADD_1Y -3997.590f
#define ADD_1Z 274.280f

#define ADD_2X 2909.130f
#define ADD_2Y -4042.970f
#define ADD_2Z 274.280f

#define ADD_3X 2861.102f
#define ADD_3Y -3997.901f
#define ADD_3Z 274.280f

bool m_bHasSporeDied;

struct MANGOS_DLL_DECL boss_loathebAI : public ScriptedAI
{
    boss_loathebAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiDeathbloomTimer;
    uint32 m_uiNecroticAuraTimer;
    uint32 m_uiInevitableDoomTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiBerserkTimer;
    uint8  m_uiNecroticAuraCount;                           // Used for emotes, 5min check

    void Reset()
    {
        m_uiDeathbloomTimer = 5000;
        m_uiNecroticAuraTimer = 12000;
        m_uiInevitableDoomTimer = MINUTE*2*IN_MILLISECONDS;
        m_uiSummonTimer = urand(10000, 15000);              // first seen in vid after approx 12s
        m_uiBerserkTimer = MINUTE*12*IN_MILLISECONDS;       // only in heroic, after 12min
        m_uiNecroticAuraCount = 0;
        m_bHasSporeDied = false;
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, DONE);

        if (!m_bHasSporeDied)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_SPORE_LOSER : ACHIEV_SPORE_LOSER_H);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LOATHEB, NOT_STARTED);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() != NPC_SPORE)
            return;

        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            pSummoned->AddThreat(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Berserk (only heroic)
        if (!m_bIsRegularMode)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_BERSERK);
                m_uiBerserkTimer = 300000;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }
        
        // Inevitable Doom
        if (m_uiInevitableDoomTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_INEVITABLE_DOOM);
            
            m_uiInevitableDoomTimer = (m_uiNecroticAuraCount <= 40) ? 30000 : 15000;
        }
        else
            m_uiInevitableDoomTimer -= uiDiff;

        // Necrotic Aura
        if (m_uiNecroticAuraTimer < uiDiff)
        {
            switch (m_uiNecroticAuraCount % 3)
            {
                case 0:
                    DoCastSpellIfCan(m_creature, SPELL_NECROTIC_AURA);
                    DoScriptText(EMOTE_AURA_BLOCKING, m_creature);
                    m_uiNecroticAuraTimer = 14000;
                    break;
                case 1:
                    DoScriptText(EMOTE_AURA_WANE, m_creature);
                    m_uiNecroticAuraTimer = 3000;
                    break;
                case 2:
                    DoScriptText(EMOTE_AURA_FADING, m_creature);
                    m_uiNecroticAuraTimer = 3000;
                    break;
            }
            m_uiNecroticAuraCount++;
        }
        else
            m_uiNecroticAuraTimer -= uiDiff;

        // Summon
        if (m_uiSummonTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_SUMMON_SPORE);
            /*
            Unit* pSummonedSpores = NULL;

            pSummonedSpores = m_creature->SummonCreature(16286,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
            pSummonedSpores = m_creature->SummonCreature(16286,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
            pSummonedSpores = m_creature->SummonCreature(16286,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
            if (pSummonedSpores)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    pSummonedSpores->AddThreat(pTarget);
            }
            */
            m_uiSummonTimer = m_bIsRegularMode ? 36000 : 18000;
        }
        else
            m_uiSummonTimer -= uiDiff;

        // Deathbloom
        if (m_uiDeathbloomTimer < uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, SPELL_DEATHBLOOM);

            m_uiDeathbloomTimer = 30000;
        }
        else
            m_uiDeathbloomTimer -= uiDiff;
            
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_loatheb_sporesAI : public ScriptedAI
{
    npc_loatheb_sporesAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 DieDelay_Timer;

    void Reset()
    {
        DieDelay_Timer = 0;
    }

    void DamageTaken(Unit* done_by, uint32 &damage)
    {
        if (damage > m_creature->GetHealth() && !DieDelay_Timer)
        {
            m_creature->CastSpell(m_creature, SPELL_FUNGAL_CREEP, true);
            m_creature->SetHealth(m_creature->GetMaxHealth());
            DieDelay_Timer = 500;
        }
        if (DieDelay_Timer)
        {
            damage = 0;
            return;
        }
    }

    void JustDied(Unit* Killer) 
    {
        if(Killer != GetClosestCreatureWithEntry(m_creature, NPC_LOATHEB, 80.0f))
            m_bHasSporeDied = true;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (DieDelay_Timer)
            if (DieDelay_Timer < diff)
            {
                m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                DieDelay_Timer = 0;
            }else DieDelay_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_loatheb(Creature* pCreature)
{
    return new boss_loathebAI(pCreature);
}

CreatureAI* GetAI_npc_loatheb_spores(Creature* pCreature)
{
    return new npc_loatheb_sporesAI(pCreature);
}

void AddSC_boss_loatheb()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_loatheb";
    NewScript->GetAI = &GetAI_boss_loatheb;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "npc_loatheb_spores";
    NewScript->GetAI = &GetAI_npc_loatheb_spores;
    NewScript->RegisterSelf();
}
