-- Snakes trap
UPDATE `creature_template` SET `ScriptName`='npc_snake_trap_serpents' WHERE `entry` IN (19921, 19833);
-- Mirror Immage
UPDATE `creature_template` SET `ScriptName`='npc_mirror_image' WHERE `entry` = 31216;
-- Rune blade
UPDATE `creature_template` SET `ScriptName`='npc_runeblade' WHERE `entry` = 27893;
-- Gargoyle
UPDATE `creature_template` SET `ScriptName` = 'mob_death_knight_gargoyle' WHERE `entry` = 27829;
-- XP eliminator
UPDATE `creature_template` SET `ScriptName` = 'npc_experience_eliminator' WHERE `entry` in (35364, 35365);