/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_ULDUAR_H
#define DEF_ULDUAR_H

enum
{
    MAX_ENCOUNTER               = 14,

    TYPE_LEVIATHAN              = 0,
    TYPE_IGNIS                  = 1,
    TYPE_RAZORSCALE             = 2,
    TYPE_XT002                  = 3,
    TYPE_ASSEMBLY               = 4,
    TYPE_KOLOGARN               = 5,
    TYPE_AURIAYA                = 6,
    TYPE_MIMIRON                = 7,
    TYPE_HODIR                  = 8,
    TYPE_THORIM                 = 9,
    TYPE_FREYA                  = 10,
    TYPE_VEZAX                  = 11,
    TYPE_YOGGSARON              = 12,
    TYPE_ALGALON                = 13,

    //other
    TYPE_RUNE_GIANT             = 14,
    TYPE_RUNIC_COLOSSUS         = 15,
    TYPE_LEVIATHAN_MK           = 16,
    TYPE_VX001                  = 17,
    TYPE_AERIAL_UNIT            = 18,
    TYPE_VISION1                = 19,
    TYPE_VISION2                = 20,
    TYPE_VISION3                = 21, 
    TYPE_YOGG_BRAIN             = 22,

    DATA_LEVIATHAN              = 14,
    DATA_IGNIS                  = 15,
    DATA_RAZORSCALE             = 16,
    DATA_XT002                  = 17,
    DATA_HEART                  = 50,
    DATA_KOLOGARN               = 18,
    DATA_AURIAYA                = 19,
    DATA_STEELBREAKER           = 20,
    DATA_MOLGEIM                = 21,
    DATA_BRUNDIR                = 22,
    DATA_MIMIRON                = 23,
    DATA_HODIR                  = 24,
    DATA_THORIM                 = 25,
    DATA_FREYA                  = 26,
    DATA_VEZAX                  = 27,
    DATA_YOGGSARON              = 28,
    DATA_ALGALON                = 29,
    DATA_RIGHT_ARM              = 30,
    DATA_LEFT_ARM               = 31,
    DATA_SENTRY_1				= 32,
    DATA_SENTRY_2				= 33,
    DATA_SENTRY_3				= 34,
    DATA_SENTRY_4				= 35,
    DATA_FERAL_DEFENDER			= 36,
    DATA_BRIGHTLEAF             = 46,
    DATA_IRONBRACH              = 47,
    DATA_STONEBARK              = 48,
    DATA_ANIMUS                 = 49,
    DATA_RED_BUTTON             = 53,
    DATA_RUNIC_COLOSSUS         = 51,
    DATA_RUNE_GIANT             = 52,
    DATA_LEVIATHAN_MK           = 54,
    DATA_VX001                  = 55,
    DATA_AERIAL_UNIT            = 56,
    DATA_HODIR_IMAGE            = 57,
    DATA_FREYA_IMAGE            = 58,
    DATA_THORIM_IMAGE           = 59,
    DATA_MIMIRON_IMAGE          = 60,
    DATA_SARA                   = 61,
    DATA_YOGG_BRAIN             = 62,
    DATA_THORIM_LEVER           = 63,
    DATA_MADNESS_DOOR           = 64,
    DATA_BRAIN_DOOR1            = 65,
    DATA_BRAIN_DOOR2            = 66,
    DATA_BRAIN_DOOR3            = 67,

    TYPE_LEVIATHAN_HARD         = 37,
    TYPE_XT002_HARD             = 38,
    TYPE_ASSEMBLY_HARD          = 39,
    TYPE_MIMIRON_HARD           = 40,
    TYPE_HODIR_HARD             = 41,
    TYPE_THORIM_HARD            = 42,
    TYPE_FREYA_HARD             = 43,
    TYPE_VEZAX_HARD             = 44,
    TYPE_YOGGSARON_HARD         = 45,

    // iron council
    TYPE_BRUNDIR                = 46,
    TYPE_MOLGEIM                = 47,
    TYPE_STEELBREAKER           = 48,

    TYPE_LEVIATHAN_TP,
    TYPE_XT002_TP,
    TYPE_MIMIRON_TP,

    NPC_LEVIATHAN               = 33113,
    NPC_IGNIS                   = 33118,
    NPC_RAZORSCALE              = 33186,
    NPC_XT002                   = 33293,
    NPC_HEART				    = 33329,
    NPC_STEELBREAKER            = 32867,
    NPC_MOLGEIM                 = 32927,
    NPC_BRUNDIR                 = 32857,
    NPC_KOLOGARN                = 32930,
    NPC_RIGHT_ARM               = 32934,
    NPC_LEFT_ARM                = 32933,
    NPC_AURIAYA                 = 33515,
    NPC_SANCTUM_SENTRY			= 34014,
    NPC_FERAL_DEFENDER			= 34035,
    NPC_MIMIRON                 = 33350,
    NPC_LEVIATHAN_MK            = 33432,
    NPC_VX001                   = 33651,
    NPC_AERIAL_UNIT             = 33670,
    NPC_HODIR                   = 32845,
    NPC_THORIM                  = 32865,
    NPC_RUNIC_COLOSSUS          = 32872,
    NPC_ANCIENT_RUNE_GIANT      = 32873,
    NPC_FREYA                   = 32906,
    NPC_BRIGHTLEAF              = 32915,
    NPC_IRONBRACH               = 32913,
    NPC_STONEBARK               = 32914,
    NPC_VEZAX                   = 33271,
    NPC_ANIMUS                  = 33524,
    NPC_YOGGSARON               = 33288,
    NPC_ALGALON                 = 32871,
    NPC_SARA                    = 33134,
    NPC_YOGG_BRAIN              = 33890, 
    // keepers images used to start the encounter
    THORIM_IMAGE                = 33878,
    MIMIRON_IMAGE               = 33880,
    HODIR_IMAGE                 = 33879,
    FREYA_IMAGE                 = 33876,

    // loot chests
    // Kologarn
    GO_KOLOGARN_BRIDGE			= 194232,
    GO_CACHE_OF_LIVING_STONE    = 195046,
    GO_CACHE_OF_LIVING_STONE_H	= 195047,
    // Hodir
    GO_CACHE_OF_WINTER          = 194307,
    GO_CACHE_OF_WINTER_H        = 194308, 
    GO_CACHE_OF_RARE_WINTER     = 194200,
    GO_CACHE_OF_RARE_WINTER_H   = 194201,
    // Thorim
    GO_CACHE_OF_STORMS          = 194312,
    GO_CACHE_OF_RARE_STORMS     = 194313,
    GO_CACHE_OF_STORMS_H        = 194314,
    GO_CACHE_OF_RARE_STORMS_H   = 194315,
    // Alagon
    GO_GIFT_OF_OBSERVER_H       = 194821,
    GO_GIFT_OF_OBSERVER         = 194822,
    GO_GIFT_OF_OBSERVER_HH      = 194823,   // unk
    // Freya -> each chest is for a mode = more elders alive = more items in chest
    // 10 man
    GO_FREYA_GIFT               = 194324,//10 normal
    GO_FREYA_GIFT_1             = 194325,//10 1 elder
    GO_FREYA_GIFT_2             = 194326,//10 2 elders
    GO_FREYA_GIFT_3             = 194327,//10 3 elders
    // 25 man
    GO_FREYA_GIFT_H             = 194328,//25 normal
    GO_FREYA_GIFT_H_1           = 194329,//25 1 elder
    GO_FREYA_GIFT_H_2           = 194330,//25 2 elder
    GO_FREYA_GIFT_H_3           = 194331,//25 3 elders
    // Mimiron
    GO_CACHE_OF_INOVATION       = 194789,
    GO_CACHE_OF_INOVATION_H     = 194956,


    // doors
    // the siege
    GO_SHIELD_WALL              = 194416,
    GO_LEVIATHAN_GATE           = 194630,
    GO_XT002_GATE               = 194631,
    // archivum
    GO_SHATTERED_DOOR           = 194553,
    GO_IRON_ENTRANCE_DOOR       = 194554,
    GO_ARCHIVUM_DOOR            = 194556,
    GO_ARCHIVUM_CONSOLE         = 194555,
    GO_CELESTIAL_ACCES          = 194628,
    GO_CELESTIAL_DOOR           = 194767,
    GO_UNIVERSE_FLOOR_ARCHIVUM  = 194715,
    GO_UNIVERSE_FLOOR_CELESTIAL = 194716,
    GO_AZEROTH_GLOBE            = 194148,
    // the keepers
    // hodir
    GO_HODIR_EXIT               = 194634,
    GO_HODIR_ICE_WALL           = 194441,
    GO_HODIR_ENTER              = 194442,
    // mimiron
    G0_MIMIRON_BUTTON           = 194739,
    GO_MIMIRON_DOOR_1           = 194774,
    GO_MIMIRON_DOOR_2           = 194775,
    GO_MIMIRON_DOOR_3           = 194776,
    GO_MIMIRON_TEL1             = 194741,
    GO_MIMIRON_TEL2             = 194742,
    GO_MIMIRON_TEL3             = 194743,
    GO_MIMIRON_TEL4             = 194744,
    GO_MIMIRON_TEL5             = 194740,
    GO_MIMIRON_TEL6             = 194746,
    GO_MIMIRON_TEL7             = 194747,
    GO_MIMIRON_TEL8             = 194748,
    GO_MIMIRON_TEL9             = 194745,
    GO_MIMIRON_ELEVATOR         = 194749,
    // Thorim
    GO_DARK_IRON_PORTCULIS      = 194560,
    GO_RUNED_STONE_DOOR         = 194557,
    GO_THORIM_STONE_DOOR        = 194558,
    GO_LIGHTNING_DOOR           = 194905,
    GO_LIGHTNING_FIELD          = 194559,
    GO_DOOR_LEVER               = 194264,
    //Yogg
    GO_ANCIENT_GATE             = 194255,
    GO_VEZAX_GATE               = 194750,
    GO_YOGG_GATE                = 194773,
    GO_BRAIN_DOOR1              = 194635,
    GO_BRAIN_DOOR2              = 194636,
    GO_BRAIN_DOOR3              = 194637,

    ACHIEV_IRON_COUNCIL         = 2888,
    ACHIEV_IRON_COUNCIL_H       = 2889,

    ACHIEV_KEEPERS              = 2890,
    ACHIEV_KEEPERS_H            = 2891,
};

#endif
