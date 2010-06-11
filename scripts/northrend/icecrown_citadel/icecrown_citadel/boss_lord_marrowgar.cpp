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
SDName: boss_lord_marrowgar
SD%Complete: 0%
SDComment:
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "icecrown_citadel.h"

enum
{
    SAY_INTRO            = -1609392,
    SAY_AGGRO            = -1609393,
    SAY_BONESTORM        = -1609394,
    SAY_BONESPIKE1       = -1609395,
    SAY_BONESPIKE2       = -1609396,
    SAY_BONESPIKE3       = -1609397,
    SAY_KILL1            = -1609398,
    SAY_KILL2            = -1609399,
    SAY_ENRAGE           = -1609401,
    SAY_DEATH            = -1609400,

    SPELL_BONE_SLICE_10     = 69055,
    SPELL_BONE_SLICE_25     = 70814,
    SPELL_COLDFLAME_10      = 69146,
    SPELL_COLDFLAME_25      = 70823,
    SPELL_COLDFLAME_10HC    = 70824,
    SPELL_COLDFLAME_25HC    = 70825,
    SPELL_COLDFLAME         = 69147,
    NPC_COLDFLAME           = 36672,
    SPELL_BONE_SPIKE        = 69057,    // just 1 spell??
    NPC_BONESPIKE           = 38711,
    SPELL_IMPALED           = 69065,
    SPELL_BONE_STORM_10     = 69075,
    SPELL_BONE_STORM_25     = 70834,
    SPELL_BONE_STORM_10HC   = 70835,
    SPELL_BONE_STORM_25HC   = 70836,
    SPELL_BERSERK           = 26662,
};

struct MANGOS_DLL_DECL boss_marrowgarAI : public ScriptedAI
{
    boss_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Difficulty = pCreature->GetMap()->GetDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Difficulty;

    uint32 m_uiSaberLash_Timer;
    uint32 m_uiColdFlame_Timer;
    uint32 m_uiBoneSpike_Timer;
    uint32 m_uiBoneStorm_Timer;
    uint32 m_uiBerserkTimer;
    bool m_bHasTaunted;

    void Reset()
    {
        m_uiSaberLash_Timer = 1000;
        m_uiColdFlame_Timer = 15000;
        m_uiBoneSpike_Timer = 30000;
        m_uiBoneStorm_Timer = 45000;
        m_uiBerserkTimer    = 600000;  // 10 min
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!m_bHasTaunted)
        {
            DoScriptText(SAY_INTRO, m_creature);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(who);   
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBoneStorm_Timer < uiDiff)
        {
            DoScriptText(SAY_BONESTORM, m_creature);

            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_BONE_STORM_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
                DoCast(m_creature->getVictim(), SPELL_BONE_STORM_25);
            if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_STORM_10HC);
            if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_STORM_25HC);
            m_uiBoneStorm_Timer = 90000;
        }
        else m_uiBoneStorm_Timer -= uiDiff;

        if (m_uiBoneSpike_Timer < uiDiff)
        {
            switch(urand(0, 2))
            {
                case 0: DoScriptText(SAY_BONESPIKE1, m_creature); break;
                case 1: DoScriptText(SAY_BONESPIKE2, m_creature); break;
                case 2: DoScriptText(SAY_BONESPIKE3, m_creature); break;
            }

            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCastSpellIfCan(target, SPELL_BONE_SPIKE);

                float fPosX, fPosY, fPosZ;
                target->GetPosition(fPosX, fPosY, fPosZ);

                m_creature->SummonCreature(NPC_BONESPIKE, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            }
            m_uiSaberLash_Timer = 4000;
            m_uiBoneSpike_Timer = 30000;
        }
        else m_uiBoneSpike_Timer -= uiDiff;

        if (m_uiColdFlame_Timer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                float fPosX, fPosY, fPosZ;
                target->GetPosition(fPosX, fPosY, fPosZ);

                m_creature->SummonCreature(NPC_COLDFLAME, fPosX, fPosY, fPosZ, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            }
            m_uiColdFlame_Timer = 15000;
        }
        else m_uiColdFlame_Timer -= uiDiff;

        if (m_uiSaberLash_Timer < uiDiff)
        {
            if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL || Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_10);
            if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL || Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
                DoCast(m_creature->getVictim(), SPELL_BONE_SLICE_25);
            m_uiSaberLash_Timer = 1000;
        }
        else m_uiSaberLash_Timer -= uiDiff;

        // berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            DoScriptText(SAY_ENRAGE, m_creature);
            m_uiBerserkTimer = 60000;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_marrowgar(Creature* pCreature)
{
    return new boss_marrowgarAI(pCreature);
}

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Difficulty = pCreature->GetMap()->GetDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 Difficulty;
    uint32 m_uiRangeCheck_Timer;
    float fPosX, fPosY, fPosZ;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(150, 200), fPosX, fPosY, fPosZ);
        m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
        SetCombatMovement(false);
        m_creature->SetSpeedRate(MOVE_RUN, 0.8f);

        if(Difficulty == RAID_DIFFICULTY_10MAN_NORMAL)
            DoCast(m_creature, SPELL_COLDFLAME_10);
        if(Difficulty == RAID_DIFFICULTY_25MAN_NORMAL)
            DoCast(m_creature, SPELL_COLDFLAME_25);
        if(Difficulty == RAID_DIFFICULTY_10MAN_HEROIC)
            DoCast(m_creature, SPELL_COLDFLAME_10HC);
        if(Difficulty == RAID_DIFFICULTY_25MAN_HEROIC)
            DoCast(m_creature, SPELL_COLDFLAME_25HC);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        if(!m_pInstance) return;
        if(type != POINT_MOTION_TYPE) return;
        if(id != 1)
             m_creature->GetMotionMaster()->MovePoint(1, fPosX, fPosY, fPosZ);
             else m_creature->ForcedDespawn();
    }

    void AttackStart(Unit *who)
    {
        //ignore all attackstart commands
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //bsw->timedCast(SPELL_COLD_FLAME_0, uiDiff);
//        bsw->timedCast(SPELL_COLD_FLAME, uiDiff);
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

struct MANGOS_DLL_DECL mob_bone_spikeAI : public ScriptedAI
{
    mob_bone_spikeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint64 m_uiVictimGUID;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
        DoCast(pWho, SPELL_IMPALED);
        m_uiVictimGUID = pWho->GetGUID();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) 
            pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);

        if (Killer)
            Killer->RemoveAurasDueToSpell(SPELL_IMPALED);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_pInstance && m_pInstance->GetData(TYPE_MARROWGAR) != IN_PROGRESS)
        {
            if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                pVictim->RemoveAurasDueToSpell(SPELL_IMPALED);
            m_creature->ForcedDespawn();
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }

};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

void AddSC_boss_marrowgar()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_marrowgar";
    NewScript->GetAI = &GetAI_boss_marrowgar;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_coldflame";
    NewScript->GetAI = &GetAI_mob_coldflame;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_bone_spike";
    NewScript->GetAI = &GetAI_mob_bone_spike;
    NewScript->RegisterSelf();
}
