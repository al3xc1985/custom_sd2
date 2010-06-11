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
SDName: Ironforge
SD%Complete: 100
SDComment: Quest support: 3702
SDCategory: Ironforge
EndScriptData */

/* ContentData
npc_royal_historian_archesonus
EndContentData */

#include "precompiled.h"

enum
{
    // King Magni Bronzebeard
    SPELL_AVATAR                    = 19135,
    SPELL_KNOCK_AWAY                = 20686,
    SPELL_STORM_BOLT                = 20685,

    // High Tinker Mekkatorque      
    SPELL_BOMB                      = 9143,
    SPELL_GOBLIN_DRAGON_GUN         = 22739,
    SPELL_SUPER_SHRINK_RAY          = 22742,
};

/*######
## npc_royal_historian_archesonus
######*/

#define GOSSIP_ITEM_ROYAL   "I am ready to listen"
#define GOSSIP_ITEM_ROYAL_1 "That is tragic. How did this happen?"
#define GOSSIP_ITEM_ROYAL_2 "Interesting, continue please."
#define GOSSIP_ITEM_ROYAL_3 "Unbelievable! How dare they??"
#define GOSSIP_ITEM_ROYAL_4 "Of course I will help!"

bool GossipHello_npc_royal_historian_archesonus(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(3702) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        pPlayer->SEND_GOSSIP_MENU(2235, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_royal_historian_archesonus(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            pPlayer->SEND_GOSSIP_MENU(2236, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(2237, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            pPlayer->SEND_GOSSIP_MENU(2238, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ROYAL_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->SEND_GOSSIP_MENU(2239, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(3702);
            break;
    }
    return true;
}

struct MANGOS_DLL_DECL boss_king_magni_bronzebreardAI : public ScriptedAI
{
    boss_king_magni_bronzebreardAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiAvatarTimer;
    uint32 m_uiKnockAwayTimer;
    uint32 m_uiStormboltTimer;

    void Reset()
    {
        m_uiAvatarTimer     = 15000;
        m_uiKnockAwayTimer  = 8000;
        m_uiStormboltTimer  = 10000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiAvatarTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_AVATAR);
            m_uiAvatarTimer = urand(15000, 12000);
        }
        else
            m_uiAvatarTimer -= uiDiff;

        if (m_uiKnockAwayTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_KNOCK_AWAY);
            m_uiKnockAwayTimer = urand(8000, 12000);
        }
        else
            m_uiKnockAwayTimer -= uiDiff;

        if (m_uiStormboltTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_STORM_BOLT);
            m_uiStormboltTimer = 10000;
        }
        else
            m_uiStormboltTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_king_magni_bronzebreard(Creature* pCreature)
{
    return new boss_king_magni_bronzebreardAI(pCreature);
}

struct MANGOS_DLL_DECL boss_high_tinker_mekkatorqueAI : public ScriptedAI
{
    boss_high_tinker_mekkatorqueAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiBombTimer;
    uint32 m_uiGoblinDragonGunTimer;
    uint32 m_uiSuperShrinkRayTimer;

    void Reset()
    {
        m_uiBombTimer               = 10000;
        m_uiGoblinDragonGunTimer    = 15000;
        m_uiSuperShrinkRayTimer     = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBombTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_BOMB);
            m_uiBombTimer = urand(8000, 15000);
        }
        else
            m_uiBombTimer -= uiDiff;

        if (m_uiGoblinDragonGunTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_GOBLIN_DRAGON_GUN);
            m_uiGoblinDragonGunTimer = urand(15000, 20000);
        }
        else
            m_uiGoblinDragonGunTimer -= uiDiff;

        if (m_uiSuperShrinkRayTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SUPER_SHRINK_RAY);
            m_uiSuperShrinkRayTimer = urand(15000, 20000);
        }
        else
            m_uiSuperShrinkRayTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_high_tinker_mekkatorque(Creature* pCreature)
{
    return new boss_high_tinker_mekkatorqueAI(pCreature);
}

void AddSC_ironforge()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_royal_historian_archesonus";
    newscript->pGossipHello =  &GossipHello_npc_royal_historian_archesonus;
    newscript->pGossipSelect = &GossipSelect_npc_royal_historian_archesonus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_king_magni_bronzebreard";
    newscript->GetAI = &GetAI_boss_king_magni_bronzebreard;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_high_tinker_mekkatorque";
    newscript->GetAI = &GetAI_boss_high_tinker_mekkatorque;
    newscript->RegisterSelf();
}