// d2itemtool.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "d2itemtool.h"
#include "itemxhg.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

#define GET_NEXT_BIT(a, b) get_d2_bit(a, bitpos, b); bitpos += b
#define SET_NEXT_BIT(a, b, c) set_d2_bit(a, bitpos, b, c); bitpos += b

static int item_types_count;
static t_d2_item_type item_types[2048];

t_d2_prop item_properties[] = {
	{0, "Strength", 1, 8, 0, 0, 0}, 
	{1, "Energy", 1, 7, 0, 0, 0}, 
	{2, "Dexterity", 1, 7, 0, 0, 0}, 
	{3, "Vitality", 1, 7, 0, 0, 0}, 
	{4, "Stat Points", 0, 0, 0, 0, 0}, 
	{5, "Skill Points", 0, 0, 0, 0, 0}, 
	{6, "Life", 0, 0, 0, 0, 0}, 
	{7, "Max Life", 1, 9, 0, 0, 0}, 
	{8, "Mana", 0, 0, 0, 0, 0}, 
	{9, "Max Mana", 1, 8, 0, 0, 0}, 
	{10, "stamina", 0, 0, 0, 0, 0}, 
	{11, "Max Stamina", 1, 8, 0, 0, 0}, 
	{12, "level", 0, 0, 0, 0, 0}, 
	{13, "experience", 0, 0, 0, 0, 0}, 
	{14, "gold", 0, 0, 0, 0, 0}, 
	{15, "goldbank", 0, 0, 0, 0, 0}, 
	{16, "Enhanced Defense %", 1, 9, 0, 0, 0}, 
	{17, "Enchanced Damage %", 2, 9, 9, 0, 0}, 
	{18, "Enchanced Min Dmg %", 1, 9, 0, 0, 0}, 
	{19, "Attack Rating", 1, 10, 0, 0, 0}, 
	{20, "Chance of Blocking %", 1, 6, 0, 0, 0}, 
	{21, "Min 1-handed Dmg", 1, 6, 0, 0, 0}, 
	{22, "Max 1-handed Dmg", 1, 7, 0, 0, 0}, 
	{23, "Min 2-handed Dmg", 1, 6, 0, 0, 0}, 
	{24, "Max 2-handed Dmg", 1, 7, 0, 0, 0}, 
	{25, "Damage % nv", 1, 8, 0, 0, 0}, 
	{26, "Regenerate Mana % nv", 1, 8, 0, 0, 0}, 
	{27, "Regenerate Mana Plus %", 1, 8, 0, 0, 0}, 
	{28, "Heal Stemina Plus %", 1, 8, 0, 0, 0}, 
	{29, "lastexp", 0, 0, 0, 0, 0}, 
	{30, "nextexp", 0, 0, 0, 0, 0}, 
	{31, "Defense", 1, 11, 0, 0, 0}, 
	{32, "Def vs Missile", 1, 9, 0, 0, 0}, 
	{33, "Def vs Melee", 1, 8, 0, 0, 0}, 
	{34, "Damage Reduced By", 1, 6, 0, 0, 0}, 
	{35, "Magic Dmg Reduced By", 1, 6, 0, 0, 0}, 
	{36, "Damage Reduced By %", 1, 8, 0, 0, 0}, 
	{37, "Magic Resistance %", 1, 8, 0, 0, 0}, 
	{38, "Max Magic Resistance %", 1, 5, 0, 0, 0}, 
	{39, "Fire Resistance %", 1, 8, 0, 0, 0}, 
	{40, "Max Fire Resistance %", 1, 5, 0, 0, 0}, 
	{41, "Lightning Resistance %", 1, 8, 0, 0, 0}, 
	{42, "Max Lightning Resistance %", 1, 5, 0, 0, 0}, 
	{43, "Cold Resistance %", 1, 8, 0, 0, 0}, 
	{44, "Max Cold Resistance %", 1, 5, 0, 0, 0}, 
	{45, "Poison Resistance %", 1, 8, 0, 0, 0}, 
	{46, "Max Poison Resistance %", 1, 5, 0, 0, 0}, 
	{47, "damageaura", 0, 0, 0, 0, 0}, 
	{48, "Fire Damage", 2, 8, 9, 0, 0}, 
	{49, "Max Fire Damage", 1, 9, 0, 0, 0}, 
	{50, "Lightning Damage", 2, 6, 10, 0, 0}, 
	{51, "Max Lightning Dmg", 1, 10, 0, 0, 0}, 
	{52, "Magic Damage", 2, 8, 9, 0, 0}, 
	{53, "Max Magic Damage", 1, 9, 0, 0, 0}, 
	{54, "Cold Damage", 3, 8, 9, 8, 0}, 
	{55, "Max Cold Damage", 1, 9, 0, 0, 0}, 
	{56, "Cold Length", 1, 8, 0, 0, 0}, 
	{57, "Poison Damage", 3, 10, 10, 9, 0}, 
	{58, "Max Poison Dmg", 1, 10, 0, 0, 0}, 
	{59, "Poison Length", 1, 9, 0, 0, 0}, 
	{60, "Life Stolen Per Hit Min", 1, 7, 0, 0, 0}, 
	{61, "Life Stolen Per Hit Max", 0, 0, 0, 0, 0}, 
	{62, "Mana Stolen Per Hit Min", 1, 7, 0, 0, 0}, 
	{63, "Mana Stolen Per Hit Max", 0, 0, 0, 0, 0}, 
	{64, "Stamina Drain Min", 0, 0, 0, 0, 0}, 
	{65, "Stamina Drain Max", 0, 0, 0, 0, 0}, 
	{66, "Stun Length", 0, 0, 0, 0, 0}, 
	{67, "Run/Walk Speed % nv", 1, 7, 0, 0, 0}, 
	{68, "Attack Speed % nv", 1, 7, 0, 0, 0}, 
	{69, "other_animrate", 0, 0, 0, 0, 0}, 
	{70, "quantity", 0, 0, 0, 0, 0}, 
	{71, "value", 1, 8, 0, 0, 0}, 
	{72, "Durabilty", 1, 9, 0, 0, 0}, 
	{73, "Max Durability", 1, 8, 0, 0, 0}, 
	{74, "Replenish Life", 1, 6, 0, 0, 0}, 
	{75, "Max Durability %", 1, 7, 0, 0, 0}, 
	{76, "Life %", 1, 6, 0, 0, 0}, 
	{77, "Mana %", 1, 6, 0, 0, 0}, 
	{78, "Attacker Takes Damage Of", 1, 7, 0, 0, 0}, 
	{79, "Extra Gold From Monsters %", 1, 9, 0, 0, 0}, 
	{80, "Chance Of Getting Magic Items %", 1, 8, 0, 0, 0}, 
	{81, "Knock Back", 1, 7, 0, 0, 0}, 
	{82, "Time Duration", 1, 9, 0, 0, 0}, 
	{83, "Add Class Skill", 2, 3, 3, 0, 0}, 
	{84, "Unsentparam1", 0, 0, 0, 0, 0}, 
	{85, "Add experience", 1, 9, 0, 0, 0}, 
	{86, "Heal after kill", 1, 7, 0, 0, 0}, 
	{87, "Reduced prices", 1, 7, 0, 0, 0}, 
	{88, "Double Herb Duration", 1, 1, 0, 0, 0}, 
	{89, "Light Radius", 1, 4, 0, 0, 0}, 
	{90, "Light Color nv", 1, 24, 0, 0, 0}, 
	{91, "Requirements %", 1, 8, 0, 0, 0}, 
	{92, "Level Require", 1, 7, 0, 0, 0}, 
	{93, "Increased Attack Speed", 1, 7, 0, 0, 0}, 
	{94, "Level Require %", 1, 7, 0, 0, 0}, 
	{95, "Last block frame", 0, 0, 0, 0, 0}, 
	{96, "Faster Run/Walk", 1, 7, 0, 0, 0}, 
	{97, "Non Class skill", 2, 9, 6, 0, 0}, 
	{98, "state", 2, 8, 1, 0, 0}, 
	{99, "Faster Hit Recovery Rate", 1, 7, 0, 0, 0}, 
	{100, "Monster player count", 0, 0, 0, 0, 0}, 
	{101, "Poison override", 0, 0, 0, 0, 0}, 
	{102, "Faster Block Rate", 1, 7, 0, 0, 0}, 
	{103, "Bypass undead", 0, 0, 0, 0, 0}, 
	{104, "Bypass demons", 0, 0, 0, 0, 0}, 
	{105, "Faster Cast Rate", 1, 7, 0, 0, 0}, 
	{106, "Bypass beasts", 0, 0, 0, 0, 0}, 
	{107, "Single Skill", 2, 9, 3, 0, 0}, 
	{108, "Rest in peace", 1, 1, 0, 0, 0}, 
	{109, "Curse resistance", 1, 9, 0, 0, 0}, 
	{110, "Poison Length Reduced By %", 1, 8, 0, 0, 0}, 
	{111, "Damage", 1, 9, 0, 0, 0}, 
	{112, "Hit Causes Monster To Flee %", 1, 7, 0, 0, 0}, 
	{113, "Hit Blinds Target", 1, 7, 0, 0, 0}, 
	{114, "Damage To Mana %", 1, 6, 0, 0, 0}, 
	{115, "Ignore Target Defense", 1, 1, 0, 0, 0}, 
	{116, "Target Defense %", 1, 7, 0, 0, 0}, 
	{117, "Prevent Monster Heal", 1, 7, 0, 0, 0}, 
	{118, "Half Freeze Duration", 1, 1, 0, 0, 0}, 
	{119, "Bonus To Attack Rating %", 1, 9, 0, 0, 0}, 
	{120, "Reduce Monster Defense per Hit", 1, 7, 0, 0, 0}, 
	{121, "Damage To Demons %", 1, 9, 0, 0, 0}, 
	{122, "Damage To Undead %", 1, 9, 0, 0, 0}, 
	{123, "Attack Rating Against Demons", 1, 10, 0, 0, 0}, 
	{124, "Attack Rating Against Undead", 1, 10, 0, 0, 0}, 
	{125, "Throwable", 1, 1, 0, 0, 0}, 
	{126, "Elemental Skill", 2, 3, 3, 0, 0}, 
	{127, "All Skill Levels", 1, 3, 0, 0, 0}, 
	{128, "Attacker Takes Lightning Dmg Of", 1, 5, 0, 0, 0}, 
	{129, "ironmaiden_level", 0, 0, 0, 0, 0}, 
	{130, "lifetap_level", 0, 0, 0, 0, 0}, 
	{131, "thorns_percent", 0, 0, 0, 0, 0}, 
	{132, "bonearmor", 0, 0, 0, 0, 0}, 
	{133, "bonearmormax", 0, 0, 0, 0, 0}, 
	{134, "Attack Freezes Target", 1, 5, 0, 0, 0}, 
	{135, "Chance of Open Wounds", 1, 7, 0, 0, 0}, 
	{136, "Chance of Crushing Blow", 1, 7, 0, 0, 0}, 
	{137, "Kick Damage", 1, 7, 0, 0, 0}, 
	{138, "Mana After Each Kill", 1, 7, 0, 0, 0}, 
	{139, "Life After Each Demon Kill", 1, 7, 0, 0, 0}, 
	{140, "Extra Blood", 1, 7, 0, 0, 0}, 
	{141, "Chance of Deadly Strike", 1, 7, 0, 0, 0}, 
	{142, "Fire Absorbs %", 1, 7, 0, 0, 0}, 
	{143, "Fire Absorbs", 1, 7, 0, 0, 0}, 
	{144, "Lightning Absorbs %", 1, 7, 0, 0, 0}, 
	{145, "Lightning Absorbs", 1, 7, 0, 0, 0}, 
	{146, "Magic Absorbs %", 1, 7, 0, 0, 0}, 
	{147, "Magic Absorbs", 1, 7, 0, 0, 0}, 
	{148, "Cold Absorbs %", 1, 7, 0, 0, 0}, 
	{149, "Cold Absorbs", 1, 7, 0, 0, 0}, 
	{150, "Slows Target By %", 1, 7, 0, 0, 0}, 
	{151, "Aura", 2, 9, 5, 0, 0}, 
	{152, "Indestructible", 1, 1, 0, 0, 0}, 
	{153, "Cannot be Frozen", 1, 1, 0, 0, 0}, 
	{154, "Slower Stamina Drain %", 1, 7, 0, 0, 0}, 
	{155, "ReAnimate", 2, 10, 7, 0, 0}, 
	{156, "Pierce Attack %", 1, 7, 0, 0, 0}, 
	{157, "Fire Magic Arrows or Bolts", 1, 7, 0, 0, 0}, 
	{158, "Fire Explosive Arrows or Bolts", 1, 7, 0, 0, 0}, 
	{159, "Min Throw Dmg nv", 1, 6, 0, 0, 0}, 
	{160, "Max Throw Dmg nv", 1, 7, 0, 0, 0}, 
	{161, "UnCoded", 0, 0, 0, 0, 0}, 
	{162, "UnCoded", 0, 0, 0, 0, 0}, 
	{163, "UnCoded", 0, 0, 0, 0, 0}, 
	{164, "UnCoded", 0, 0, 0, 0, 0}, 
	{165, "UnCoded", 0, 0, 0, 0, 0}, 
	{166, "UnCoded", 0, 0, 0, 0, 0}, 
	{167, "UnCoded", 0, 0, 0, 0, 0}, 
	{168, "UnCoded", 0, 0, 0, 0, 0}, 
	{169, "UnCoded", 0, 0, 0, 0, 0}, 
	{170, "UnCoded", 0, 0, 0, 0, 0}, 
	{171, "UnCoded", 0, 0, 0, 0, 0}, 
	{172, "UnCoded", 0, 0, 0, 0, 0}, 
	{173, "UnCoded", 0, 0, 0, 0, 0}, 
	{174, "UnCoded", 0, 0, 0, 0, 0}, 
	{175, "UnCoded", 0, 0, 0, 0, 0}, 
	{176, "UnCoded", 0, 0, 0, 0, 0}, 
	{177, "UnCoded", 0, 0, 0, 0, 0}, 
	{178, "UnCoded", 0, 0, 0, 0, 0}, 
	{179, "Attack Vs Monster", 2, 10, 9, 0, 0}, 
	{180, "Damage Vs Monster", 2, 10, 9, 0, 0}, 
	{181, "fade", 1, 3, 0, 0, 0}, 
	{182, "Armor override %", 0, 0, 0, 0, 0}, 
	{183, "Unused183", 0, 0, 0, 0, 0}, 
	{184, "Unused184", 0, 0, 0, 0, 0}, 
	{185, "Unused185", 0, 0, 0, 0, 0}, 
	{186, "Unused186", 0, 0, 0, 0, 0}, 
	{187, "Unused187", 0, 0, 0, 0, 0}, 
	{188, "Add Skill Tab", 3, 3, 13, 3, 0}, 
	{189, "Unused189", 0, 0, 0, 0, 0}, 
	{190, "Unused190", 0, 0, 0, 0, 0}, 
	{191, "Unused191", 0, 0, 0, 0, 0}, 
	{192, "Unused192", 0, 0, 0, 0, 0}, 
	{193, "Unused193", 0, 0, 0, 0, 0}, 
	{194, "Increased Sockets", 1, 4, 0, 0, 0}, 
	{195, "Skill On Attack", 3, 6, 10, 7, 0}, 
	{196, "Skill On Kill", 3, 6, 10, 7, 0}, 
	{197, "Skill On Death", 3, 6, 10, 7, 0}, 
	{198, "Skill On Hit", 3, 6, 10, 7, 0}, 
	{199, "Skill On Levelup", 3, 6, 10, 7, 0}, 
	{200, "Unused200", 0, 0, 0, 0, 0}, 
	{201, "Skill On Get Hit", 3, 6, 10, 7, 0}, 
	{202, "Unused202", 0, 0, 0, 0, 0}, 
	{203, "Unused203", 0, 0, 0, 0, 0}, 
	{204, "Charged Skill", 4, 6, 10, 8, 8}, 
	{205, "Unused205", 0, 0, 0, 0, 0}, 
	{206, "Unused206", 0, 0, 0, 0, 0}, 
	{207, "Unused207", 0, 0, 0, 0, 0}, 
	{208, "Unused208", 0, 0, 0, 0, 0}, 
	{209, "Unused209", 0, 0, 0, 0, 0}, 
	{210, "Unused210", 0, 0, 0, 0, 0}, 
	{211, "Unused211", 0, 0, 0, 0, 0}, 
	{212, "Unused212", 0, 0, 0, 0, 0}, 
	{213, "Unused213", 0, 0, 0, 0, 0}, 
	{214, "Defense (on Lvl)", 1, 6, 0, 0, 0}, 
	{215, "Enhanced Defense (on Lvl)", 1, 6, 0, 0, 0}, 
	{216, "Life (on Lvl)", 1, 6, 0, 0, 0}, 
	{217, "Mana (on Lvl)", 1, 6, 0, 0, 0}, 
	{218, "Maximum Damage (on Lvl)", 1, 6, 0, 0, 0}, 
	{219, "Enhanced MaxDmg (on Lvl)", 1, 6, 0, 0, 0}, 
	{220, "Strength (on Lvl)", 1, 6, 0, 0, 0}, 
	{221, "Dexterity (on Lvl)", 1, 6, 0, 0, 0}, 
	{222, "Energy (on Lvl)", 1, 6, 0, 0, 0}, 
	{223, "Vitality (on Lvl)", 1, 6, 0, 0, 0}, 
	{224, "Att Rating (on Lvl)", 1, 6, 0, 0, 0}, 
	{225, "Bonus to Att Rating % (on Lvl)", 1, 6, 0, 0, 0}, 
	{226, "Maximum Cold Dmg (on Lvl)", 1, 6, 0, 0, 0}, 
	{227, "Maximum Fire Dmg (on Lvl)", 1, 6, 0, 0, 0}, 
	{228, "Maximum Lightning Dmg (on Lvl)", 1, 6, 0, 0, 0}, 
	{229, "Maximum Poison Dmg (on Lvl)", 1, 6, 0, 0, 0}, 
	{230, "Cold Resistance % (on Lvl)", 1, 6, 0, 0, 0}, 
	{231, "Fire Resistance % (on Lvl)", 1, 6, 0, 0, 0}, 
	{232, "Lightning Resistance % (on Lvl)", 1, 6, 0, 0, 0}, 
	{233, "Poison Resistance % (on Lvl)", 1, 6, 0, 0, 0}, 
	{234, "Absorbs Cold Damage (on Lvl)", 1, 6, 0, 0, 0}, 
	{235, "Absorbs Fire Damage (on Lvl)", 1, 6, 0, 0, 0}, 
	{236, "Absorbs Lightning Damage (on Lvl)", 1, 6, 0, 0, 0}, 
	{237, "Absorbs Poison (per lvl)", 1, 6, 0, 0, 0}, 
	{238, "Attacker Takes Damage of (on Lvl)", 1, 5, 0, 0, 0}, 
	{239, "Extra Gold From Monsters % (on Lvl)", 1, 6, 0, 0, 0}, 
	{240, "Chance of Getting Magic Items % (on Lvl)", 1, 6, 0, 0, 0}, 
	{241, "Heal Stamina Plus % (on Lvl)", 1, 6, 0, 0, 0}, 
	{242, "Maximum Stamina (on Lvl)", 1, 6, 0, 0, 0}, 
	{243, "Damage to Demons % (on Lvl)", 1, 6, 0, 0, 0}, 
	{244, "Damage to Undead % (on Lvl)", 1, 6, 0, 0, 0}, 
	{245, "Att Rating against Demons (on Lvl)", 1, 6, 0, 0, 0}, 
	{246, "Att Rating against Undead (on Lvl)", 1, 6, 0, 0, 0}, 
	{247, "Chance of Crushing Blow (on Lvl)", 1, 6, 0, 0, 0}, 
	{248, "Chance of Open Wounds (on Lvl)", 1, 6, 0, 0, 0}, 
	{249, "Kick Damage (on Lvl)", 1, 6, 0, 0, 0}, 
	{250, "Chance of Deadly Strike (on Lvl)", 1, 6, 0, 0, 0}, 
	{251, "Find Gems per level", 0, 0, 0, 0, 0}, 
	{252, "Repair 1 Durability In Seconds", 1, 6, 0, 0, 0}, 
	{253, "Replenish 1 Quantity In Seconds", 1, 6, 0, 0, 0}, 
	{254, "Increased Stack Size", 1, 8, 0, 0, 0}, 
	{255, "UnCoded", 0, 0, 0, 0, 0}, 
	{256, "UnCoded", 0, 0, 0, 0, 0}, 
	{257, "UnCoded", 0, 0, 0, 0, 0}, 
	{258, "UnCoded", 0, 0, 0, 0, 0}, 
	{259, "UnCoded", 0, 0, 0, 0, 0}, 
	{260, "UnCoded", 0, 0, 0, 0, 0}, 
	{261, "UnCoded", 0, 0, 0, 0, 0}, 
	{262, "UnCoded", 0, 0, 0, 0, 0}, 
	{263, "UnCoded", 0, 0, 0, 0, 0}, 
	{264, "UnCoded", 0, 0, 0, 0, 0}, 
	{265, "UnCoded", 0, 0, 0, 0, 0}, 
	{266, "UnCoded", 0, 0, 0, 0, 0}, 
	{267, "UnCoded", 0, 0, 0, 0, 0}, 
	{268, "Defense (by Time) [1]", 3, 2, 10, 10, 0}, 
	{269, "Defense (by Time) [2]", 3, 2, 10, 10, 0}, 
	{270, "Life (by Time)", 3, 2, 10, 10, 0}, 
	{271, "Mana (by Time)", 3, 2, 10, 10, 0}, 
	{272, "Max Damage (by Time) [1]", 3, 2, 10, 10, 0}, 
	{273, "Max Damage (by Time) [2]", 3, 2, 10, 10, 0}, 
	{274, "Strength (by Time)", 3, 2, 10, 10, 0}, 
	{275, "Dexterity (by Time)", 3, 2, 10, 10, 0}, 
	{276, "Energy (by Time)", 3, 2, 10, 10, 0}, 
	{277, "Vitality (by Time)", 3, 2, 10, 10, 0}, 
	{278, "Attack Rating (by Time) [1]", 3, 2, 10, 10, 0}, 
	{279, "Attack Rating (by Time) [2]", 3, 2, 10, 10, 0}, 
	{280, "Max Cold Dmg (by Time)", 3, 2, 10, 10, 0}, 
	{281, "Max Fire Dmg (by Time)", 3, 2, 10, 10, 0}, 
	{282, "Max Lightning Dmg (by Time)", 3, 2, 10, 10, 0}, 
	{283, "Max Poison Dmg (by Time)", 3, 2, 10, 10, 0}, 
	{284, "Cold Resistance % (by Time)", 3, 2, 10, 10, 0}, 
	{285, "Fire Resistance % (by Time)", 3, 2, 10, 10, 0}, 
	{286, "Lightning Resistance % (by Time)", 3, 2, 10, 10, 0}, 
	{287, "Poison Resistance % (by Time)", 3, 2, 10, 10, 0}, 
	{288, "Absorbs Cold Damage (by Time)", 3, 2, 10, 10, 0}, 
	{289, "Absorbs Fire Damage (by Time)", 3, 2, 10, 10, 0}, 
	{290, "Absorbs Lightning Damage (by Time)", 3, 2, 10, 10, 0}, 
	{291, "Blessed Aim", 3, 2, 10, 10, 0}, 
	{292, "Gold From Monsters % (by Time)", 3, 2, 10, 10, 0}, 
	{293, "Find Magic Items % (by Time)", 3, 2, 10, 10, 0}, 
	{294, "Heal Stamina Plus % (by Time)", 3, 2, 10, 10, 0}, 
	{295, "Max Stamina (by Time)", 3, 2, 10, 10, 0}, 
	{296, "Damage vs Demon % (by Time)", 3, 2, 10, 10, 0}, 
	{297, "Damage vs Undead % (by Time)", 3, 2, 10, 10, 0}, 
	{298, "Attack Rating vs Demon (by Time)", 3, 2, 10, 10, 0}, 
	{299, "Attack Rating vs Undead (by Time)", 3, 2, 10, 10, 0}, 
	{300, "Chance of Crushing Blow (by Time)", 3, 2, 10, 10, 0}, 
	{301, "Chance of Open Wounds (by Time)", 3, 2, 10, 10, 0}, 
	{302, "Kick Damage (by Time)", 3, 2, 10, 10, 0}, 
	{303, "Chance of Deadly Strike (by Time)", 3, 2, 10, 10, 0}, 
	{304, "item_find_gems_bytime", 0, 0, 0, 0, 0}, 
	{305, "item_pierce_cold", 1, 8, 0, 0, 0}, 
	{306, "item_pierce_fire", 1, 8, 0, 0, 0}, 
	{307, "item_pierce_ltng", 1, 8, 0, 0, 0}, 
	{308, "item_pierce_pois", 1, 8, 0, 0, 0}, 
	{309, "item_damage_vs_monster", 0, 0, 0, 0, 0}, 
	{310, "item_damage_percent_vs_monster", 0, 0, 0, 0, 0}, 
	{311, "item_tohit_vs_monster", 0, 0, 0, 0, 0}, 
	{312, "item_tohit_percent_vs_monster", 0, 0, 0, 0, 0}, 
	{313, "item_ac_vs_monster", 0, 0, 0, 0, 0}, 
	{314, "item_ac_percent_vs_monster", 0, 0, 0, 0, 0}, 
	{315, "firelength", 0, 0, 0, 0, 0}, 
	{316, "burningmin", 0, 0, 0, 0, 0}, 
	{317, "burningmax", 0, 0, 0, 0, 0}, 
	{318, "progressive_damage", 0, 0, 0, 0, 0}, 
	{319, "progressive_steal", 0, 0, 0, 0, 0}, 
	{320, "progressive_other", 0, 0, 0, 0, 0}, 
	{321, "progressive_fire", 0, 0, 0, 0, 0}, 
	{322, "progressive_cold", 0, 0, 0, 0, 0}, 
	{323, "progressive_lightning", 0, 0, 0, 0, 0}, 
	{324, "item_extra_charges", 1, 6, 0, 0, 0}, 
	{325, "progressive_tohit", 0, 0, 0, 0, 0}, 
	{326, "poison_count", 0, 0, 0, 0, 0}, 
	{327, "damage_framerate", 0, 0, 0, 0, 0}, 
	{328, "pierce_idx", 0, 0, 0, 0, 0}, 
	{329, "passive_fire_mastery", 1, 9, 0, 0, 0}, 
	{330, "passive_ltng_mastery", 1, 9, 0, 0, 0}, 
	{331, "passive_cold_mastery", 1, 9, 0, 0, 0}, 
	{332, "passive_pois_mastery", 1, 9, 0, 0, 0}, 
	{333, "passive_fire_pierce", 1, 8, 0, 0, 0}, 
	{334, "passive_ltng_pierce", 1, 8, 0, 0, 0}, 
	{335, "passive_cold_pierce", 1, 8, 0, 0, 0}, 
	{336, "passive_pois_pierce", 1, 8, 0, 0, 0}, 
	{337, "passive_critical_strike", 1, 8, 0, 0, 0}, 
	{338, "passive_dodge", 1, 7, 0, 0, 0}, 
	{339, "passive_avoid", 1, 7, 0, 0, 0}, 
	{340, "passive_evade", 1, 7, 0, 0, 0}, 
	{341, "passive_warmth", 1, 8, 0, 0, 0}, 
	{342, "passive_mastery_melee_th", 1, 8, 0, 0, 0}, 
	{343, "passive_mastery_melee_dmg", 1, 8, 0, 0, 0}, 
	{344, "passive_mastery_melee_crit", 1, 8, 0, 0, 0}, 
	{345, "passive_mastery_throw_th", 1, 8, 0, 0, 0}, 
	{346, "passive_mastery_throw_dmg", 1, 8, 0, 0, 0}, 
	{347, "passive_mastery_throw_crit", 1, 8, 0, 0, 0}, 
	{348, "passive_weaponblock", 1, 8, 0, 0, 0}, 
	{349, "passive_summon_resist", 1, 8, 0, 0, 0}, 
	{350, "modifierlist_skill", 0, 0, 0, 0, 0}, 
	{351, "modifierlist_level", 0, 0, 0, 0, 0}, 
	{352, "last_sent_hp_pct", 0, 0, 0, 0, 0}, 
	{353, "source_unit_type", 0, 0, 0, 0, 0}, 
	{354, "source_unit_id", 0, 0, 0, 0, 0}, 
	{355, "shortparam1", 0, 0, 0, 0, 0}, 
	{356, "questitemdifficulty", 1, 2, 0, 0, 0}, 
	{357, "passive_mag_mastery", 1, 9, 0, 0, 0}, 
	{358, "passive_mag_pierce", 1, 8, 0, 0, 0}
};

unsigned int bit_transposition(const unsigned int in, const int len){
	int i;
	unsigned int result = 0;
	for(i = 0; i < len; i ++){
		result = (result << 1);
		result += ((in & (1 << i))) ? 1 : 0;
	}
	return result;
}

unsigned int get_d2_bit(const char* pos, const int start, const int len){
	int result = 0;
	int i;
	for(i = start; i < start + len; i ++){
		unsigned char* cur = (unsigned char*)pos + (i / 8);
		result = (result << 1);
		result += (*cur & (1 << (i % 8))) ? 1 : 0;
	}
	return bit_transposition(result, len);
}

void set_d2_bit(char* pos, const int start, const int len, const int val){
	int fp = 0;
	int i;
	int tmp = bit_transposition(val, len);
	for(i = start; i < start + len; i ++){
		unsigned char* cur = (unsigned char*)pos + (i / 8);
		if (((tmp >> (len - (fp+1))) & 1) > 0)
		{
			*cur |= (1 << (i % 8));
		}
		fp++;
	}
}

t_d2_item_type* item_get_type(const char* item_code)
{
	for (int i = 0; i < item_types_count; i ++)
	{
		if (memcmp(item_types[i].type, "rin", 3) == 0)
		{
			int z=  0;
		}
		if (memcmp(item_code, item_types[i].type, 3) == 0)
			return &item_types[i];
	}
	return NULL;
}

int item_readprop(char* item, int &bitpos, t_d2_item_prop_list* dst)
{
	int c_prop = 0;
	while (1)
	{
		int code = GET_NEXT_BIT(item, 9);
		if (code == 0x1ff) break;
		if (c_prop > 64) return -1;
		t_d2_prop* prop = &item_properties[code];
		dst->props[c_prop].prop_code = prop->code;
		for (int j = 0; j < prop->param_count; j ++)
		{
			dst->props[c_prop].prop_args[j] = GET_NEXT_BIT(item, prop->param_len[j]);
		}
		c_prop ++;
	}
	dst->count = c_prop;
	return 0;
}

int item_writeprop(char* item, int &bitpos, const t_d2_item_prop_list* dst)
{
	for (int i = 0; i < dst->count; i ++)
	{
		SET_NEXT_BIT(item, 9, dst->props[i].prop_code);
		t_d2_prop* prop = &item_properties[dst->props[i].prop_code];
		for (int j = 0; j < prop->param_count; j ++)
		{
			SET_NEXT_BIT(item, prop->param_len[j], dst->props[i].prop_args[j]);
		}
	}
	SET_NEXT_BIT(item, 9, 0x1ff);
	return 0;
}

int item_struct2raw(const t_d2_item_struct* src, char* dst)
{
	int bitpos = 0;
	SET_NEXT_BIT(dst, 8, src->majic[0]);
	SET_NEXT_BIT(dst, 8, src->majic[1]);
	SET_NEXT_BIT(dst, 1, src->quest_item);
	SET_NEXT_BIT(dst, 3, src->unk21);
	SET_NEXT_BIT(dst, 1, src->identified);
	SET_NEXT_BIT(dst, 3, src->unk25);
	SET_NEXT_BIT(dst, 2, src->unk30);
	SET_NEXT_BIT(dst, 1, src->duplicated);
	SET_NEXT_BIT(dst, 1, src->socketed);
	SET_NEXT_BIT(dst, 2, src->unk34);
	SET_NEXT_BIT(dst, 1, src->illegal);
	SET_NEXT_BIT(dst, 1, src->unk37);
	SET_NEXT_BIT(dst, 1, src->ear);
	SET_NEXT_BIT(dst, 1, src->starter);
	SET_NEXT_BIT(dst, 2, src->unk42);
	SET_NEXT_BIT(dst, 1, src->unk44);
	SET_NEXT_BIT(dst, 1, src->simple);
	SET_NEXT_BIT(dst, 1, src->ethereal);
	SET_NEXT_BIT(dst, 1, src->unk47);
	SET_NEXT_BIT(dst, 1, src->inscribed);
	SET_NEXT_BIT(dst, 1, src->unk51);
	SET_NEXT_BIT(dst, 1, src->rune_word);
	SET_NEXT_BIT(dst, 5, src->unk53);
	SET_NEXT_BIT(dst, 8, src->unk60);
	SET_NEXT_BIT(dst, 2, src->unk70);
	SET_NEXT_BIT(dst, 3, src->location);
	SET_NEXT_BIT(dst, 4, src->position);
	SET_NEXT_BIT(dst, 4, src->col);
	SET_NEXT_BIT(dst, 4, src->row);
	SET_NEXT_BIT(dst, 3, src->stored_loc);

	if (src->ear)
	{
		//fill ear structure
		SET_NEXT_BIT(dst, 3, src->ear_format.ear_class);
		SET_NEXT_BIT(dst, 7, src->ear_format.ear_level);
		for (int i = 0; i < 18; i ++)
		{
			SET_NEXT_BIT(dst, 8, src->ear_format.player[i]);
			if (src->ear_format.player[i] == 0) break;
		}
	}

	if (!src->ear)
	{
		for (int i = 0; i < 4; i ++)
		{
			SET_NEXT_BIT(dst, 8, src->info_format.type[i]);
		}
		if (!src->simple)
		{
			//fill extended structure
			SET_NEXT_BIT(dst, 3, src->info_format.extended_format.gems);
			SET_NEXT_BIT(dst, 32, src->info_format.extended_format.guid);
			SET_NEXT_BIT(dst, 7, src->info_format.extended_format.drop_level);
			SET_NEXT_BIT(dst, 4, src->info_format.extended_format.quality);
			SET_NEXT_BIT(dst, 1, src->info_format.extended_format.vargfx);
			if (src->info_format.extended_format.vargfx)
			{
				SET_NEXT_BIT(dst, 3, src->info_format.extended_format.n_vargfx);
			}
			SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_class);
			if (src->info_format.extended_format.has_class)
			{
				SET_NEXT_BIT(dst, 11, src->info_format.extended_format.class_info);
			}
			if (strncmp(src->info_format.type, "ibk", 3) == 0 || strncmp(src->info_format.type, "tbk", 3) == 0)
			{
				SET_NEXT_BIT(dst, 5, src->info_format.extended_format.spell_id);
			}
			if (src->info_format.extended_format.quality == 1)
			{
				SET_NEXT_BIT(dst, 3, src->info_format.extended_format.low_quality);
			}
			if (src->info_format.extended_format.quality == 3)
			{
				SET_NEXT_BIT(dst, 3, src->info_format.extended_format.high_quality);
			}
			//quality 4: magic
			if (src->info_format.extended_format.quality == 4)
			{
				SET_NEXT_BIT(dst, 11, src->info_format.extended_format.magic_prefix);
				SET_NEXT_BIT(dst, 11, src->info_format.extended_format.magic_suffix);
			}
			//quality 5: set
			if (src->info_format.extended_format.quality == 5)
			{
				SET_NEXT_BIT(dst, 12, src->info_format.extended_format.set_id);
			}
			//quality 6, 8
			if (src->info_format.extended_format.quality == 6 || src->info_format.extended_format.quality == 8)
			{
				SET_NEXT_BIT(dst, 8, src->info_format.extended_format.rare_name_1);
				SET_NEXT_BIT(dst, 8, src->info_format.extended_format.rare_name_2);
				SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_prefix_1);
				if (src->info_format.extended_format.has_prefix_1)
				{
					SET_NEXT_BIT(dst, 11, src->info_format.extended_format.prefix_1);
				}
				SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_suffix_1);
				if (src->info_format.extended_format.has_suffix_1)
				{
					SET_NEXT_BIT(dst, 11, src->info_format.extended_format.suffix_1);
				}
				SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_prefix_2);
				if (src->info_format.extended_format.has_prefix_2)
				{
					SET_NEXT_BIT(dst, 11, src->info_format.extended_format.prefix_2);
				}
				SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_suffix_2);
				if (src->info_format.extended_format.has_suffix_2)
				{
					SET_NEXT_BIT(dst, 11, src->info_format.extended_format.suffix_2);
				}
				SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_prefix_3);
				if (src->info_format.extended_format.has_prefix_3)
				{
					SET_NEXT_BIT(dst, 11, src->info_format.extended_format.prefix_3);
				}
				SET_NEXT_BIT(dst, 1, src->info_format.extended_format.has_suffix_3);
				if (src->info_format.extended_format.has_suffix_3)
				{
					SET_NEXT_BIT(dst, 11, src->info_format.extended_format.suffix_3);
				}
			}
			//quality 7: unique
			if (src->info_format.extended_format.quality == 7)
			{
				SET_NEXT_BIT(dst, 12, src->info_format.extended_format.unique_id);
			}
			if (src->rune_word)
			{
				SET_NEXT_BIT(dst, 16, src->info_format.extended_format.rune_word);
			}
			if (src->inscribed)
			{
				for (int i = 0; i < 16; i ++)
				{
					SET_NEXT_BIT(dst, 7, src->info_format.extended_format.inscribed_name[i]);
					if (src->info_format.extended_format.inscribed_name[i] == 0) break;
				}
			}
			//FIXME: what does sType == book stand for?
		}
		SET_NEXT_BIT(dst, 1, src->info_format.has_rand);
		if (src->info_format.has_rand)
		{
			SET_NEXT_BIT(dst, 1, src->info_format.pad1);
			SET_NEXT_BIT(dst, 1, src->info_format.pad2);
			SET_NEXT_BIT(dst, 1, src->info_format.pad3);
		}
		if (!src->simple && !src->ear)
		{
			t_d2_item_type* t = item_get_type(src->info_format.type);
			//fill specified struct
			if (t->catalog == 0)
			{
				SET_NEXT_BIT(dst, 11, src->info_format.specified_format.def);
			}
			if (t->catalog == 0 || t->catalog == 1)
			{
				SET_NEXT_BIT(dst, 8, src->info_format.specified_format.max_dur);
				if (src->info_format.specified_format.max_dur != 0)
				{
					SET_NEXT_BIT(dst, 9, src->info_format.specified_format.cur_dur);
				}
			}
			if (src->socketed)
			{
				SET_NEXT_BIT(dst, 4, src->info_format.specified_format.socket_num);
			}
			if (t->stackable)
			{
				SET_NEXT_BIT(dst, 9, src->info_format.specified_format.quantity);
			}
			if (src->info_format.extended_format.quality == 5)
			{
				for (int i = 0; i < 5; i ++)
				{
					SET_NEXT_BIT(dst, 1, src->info_format.specified_format.has_set_prop[i]);
				}
			}
			item_writeprop(dst, bitpos, &src->info_format.specified_format.props);
			for (int i = 0; i < 5; i ++)
			{
				if (src->info_format.specified_format.has_set_prop[i])
				{
					item_writeprop(dst, bitpos, &src->info_format.specified_format.set_props[i]);
				}
			}
			if (src->rune_word)
			{
				item_writeprop(dst, bitpos, &src->info_format.specified_format.rw_props);
			}
		}
	}
	return (bitpos / 8) + ((bitpos % 8 != 0) ? 1 : 0);
}

int item_raw2struct(char* item, t_d2_item_struct* dst)
{
	int bitpos = 0;
	if (item[0] != 'J' || item[1] != 'M') return 0;
	memset(dst, 0x00, sizeof(t_d2_item_struct));
	dst->majic[0] = GET_NEXT_BIT(item, 8);
	dst->majic[1] = GET_NEXT_BIT(item, 8);
	dst->quest_item = GET_NEXT_BIT(item, 1);
	dst->unk21 = GET_NEXT_BIT(item, 3);
	dst->identified = GET_NEXT_BIT(item, 1);
	dst->unk25 = GET_NEXT_BIT(item, 3);
	dst->unk30 = GET_NEXT_BIT(item, 2);
	dst->duplicated = GET_NEXT_BIT(item, 1);
	dst->socketed = GET_NEXT_BIT(item, 1);
	dst->unk34 = GET_NEXT_BIT(item, 2);
	dst->illegal = GET_NEXT_BIT(item, 1);
	dst->unk37 = GET_NEXT_BIT(item, 1);
	dst->ear = GET_NEXT_BIT(item, 1);
	dst->starter = GET_NEXT_BIT(item, 1);
	dst->unk42 = GET_NEXT_BIT(item, 2);
	dst->unk44 = GET_NEXT_BIT(item, 1);
	dst->simple = GET_NEXT_BIT(item, 1);
	dst->ethereal = GET_NEXT_BIT(item, 1);
	dst->unk47 = GET_NEXT_BIT(item, 1);
	dst->inscribed = GET_NEXT_BIT(item, 1);
	dst->unk51 = GET_NEXT_BIT(item, 1);
	dst->rune_word = GET_NEXT_BIT(item, 1);
	dst->unk53 = GET_NEXT_BIT(item, 5);
	dst->unk60 = GET_NEXT_BIT(item, 8);
	dst->unk70 = GET_NEXT_BIT(item, 2);
	dst->location = GET_NEXT_BIT(item, 3);
	dst->position = GET_NEXT_BIT(item, 4);
	dst->col = GET_NEXT_BIT(item, 4);
	dst->row = GET_NEXT_BIT(item, 4);
	dst->stored_loc = GET_NEXT_BIT(item, 3);
	if (dst->ear)
	{
		//fill ear structure
		dst->ear_format.ear_class = GET_NEXT_BIT(item, 3);
		dst->ear_format.ear_level = GET_NEXT_BIT(item, 7);
		for (int i = 0; i < 18; i ++)
		{
			dst->ear_format.player[i] = GET_NEXT_BIT(item, 7);
			if (dst->ear_format.player[i] == 0) break;
		}
		dst->ear_format.player[18] = 0;
		strcpy(dst->info_format.type, "ear");
	}
	if (!dst->ear)
	{
		for (int i = 0; i < 4; i ++)
		{
			dst->info_format.type[i] = GET_NEXT_BIT(item, 8);
		}
		dst->info_format.type[4] = 0;
		if (!dst->simple)
		{
			//fill extended structure
			dst->info_format.extended_format.gems = GET_NEXT_BIT(item, 3);
			dst->info_format.extended_format.guid = GET_NEXT_BIT(item, 32);
			dst->info_format.extended_format.drop_level = GET_NEXT_BIT(item, 7);
			dst->info_format.extended_format.quality = GET_NEXT_BIT(item, 4);
			dst->info_format.extended_format.vargfx = GET_NEXT_BIT(item, 1);
			if (dst->info_format.extended_format.vargfx)
			{
				dst->info_format.extended_format.n_vargfx = GET_NEXT_BIT(item, 3);
			}
			dst->info_format.extended_format.has_class = GET_NEXT_BIT(item, 1);
			if (dst->info_format.extended_format.has_class)
			{
				dst->info_format.extended_format.class_info = GET_NEXT_BIT(item, 11);
			}
			if (strncmp(dst->info_format.type, "ibk", 3) == 0 || strncmp(dst->info_format.type, "tbk", 3) == 0)
			{
				dst->info_format.extended_format.spell_id = GET_NEXT_BIT(item, 5);
			}
			if (dst->info_format.extended_format.quality == 1)
			{
				dst->info_format.extended_format.low_quality = GET_NEXT_BIT(item, 3);
			}
			if (dst->info_format.extended_format.quality == 3)
			{
				dst->info_format.extended_format.high_quality = GET_NEXT_BIT(item, 3);
			}
			//quality 4: magic
			if (dst->info_format.extended_format.quality == 4)
			{
				dst->info_format.extended_format.magic_prefix = GET_NEXT_BIT(item, 11);
				dst->info_format.extended_format.magic_suffix = GET_NEXT_BIT(item, 11);
			}
			//quality 5: set
			if (dst->info_format.extended_format.quality == 5)
			{
				dst->info_format.extended_format.set_id = GET_NEXT_BIT(item, 12);
			}
			//quality 6, 8
			if (dst->info_format.extended_format.quality == 6 || dst->info_format.extended_format.quality == 8)
			{
				dst->info_format.extended_format.rare_name_1 = GET_NEXT_BIT(item, 8);
				dst->info_format.extended_format.rare_name_2 = GET_NEXT_BIT(item, 8);
				dst->info_format.extended_format.has_prefix_1 = GET_NEXT_BIT(item, 1);
				if (dst->info_format.extended_format.has_prefix_1)
				{
					dst->info_format.extended_format.prefix_1 = GET_NEXT_BIT(item, 11);
				}
				dst->info_format.extended_format.has_suffix_1 = GET_NEXT_BIT(item, 1);
				if (dst->info_format.extended_format.has_suffix_1)
				{
					dst->info_format.extended_format.suffix_1 = GET_NEXT_BIT(item, 11);
				}
				dst->info_format.extended_format.has_prefix_2 = GET_NEXT_BIT(item, 1);
				if (dst->info_format.extended_format.has_prefix_2)
				{
					dst->info_format.extended_format.prefix_2 = GET_NEXT_BIT(item, 11);
				}
				dst->info_format.extended_format.has_suffix_2 = GET_NEXT_BIT(item, 1);
				if (dst->info_format.extended_format.has_suffix_2)
				{
					dst->info_format.extended_format.suffix_2 = GET_NEXT_BIT(item, 11);
				}
				dst->info_format.extended_format.has_prefix_3 = GET_NEXT_BIT(item, 1);
				if (dst->info_format.extended_format.has_prefix_3)
				{
					dst->info_format.extended_format.prefix_3 = GET_NEXT_BIT(item, 11);
				}
				dst->info_format.extended_format.has_suffix_3 = GET_NEXT_BIT(item, 1);
				if (dst->info_format.extended_format.has_suffix_3)
				{
					dst->info_format.extended_format.suffix_3 = GET_NEXT_BIT(item, 11);
				}
			}
			//quality 7: unique
			if (dst->info_format.extended_format.quality == 7)
			{
				dst->info_format.extended_format.unique_id = GET_NEXT_BIT(item, 12);
			}
			if (dst->rune_word)
			{
				dst->info_format.extended_format.rune_word = GET_NEXT_BIT(item, 16);
			}
			if (dst->inscribed)
			{
				for (int i = 0; i < 15; i ++)
				{
					dst->info_format.extended_format.inscribed_name[i] = GET_NEXT_BIT(item, 7);
					if (dst->info_format.extended_format.inscribed_name[i] == 0) break;
				}
				dst->info_format.extended_format.inscribed_name[15] = 0;
			}
			//FIXME: what does sType == book stand for?
		}
		dst->info_format.has_rand = GET_NEXT_BIT(item, 1);
		if (dst->info_format.has_rand)
		{
			dst->info_format.pad1 = GET_NEXT_BIT(item, 1);
			dst->info_format.pad2 = GET_NEXT_BIT(item, 1);
			dst->info_format.pad3 = GET_NEXT_BIT(item, 1);
		}
	}
	if (!dst->simple && !dst->ear)
	{
		//fill specified struct
		t_d2_item_type* t = item_get_type(dst->info_format.type);
		if (!t)
		{
			memset(dst, 0, sizeof(struct t_d2_item_struct));
			return 0;
		}
		if (t->catalog == 0)
		{
			dst->info_format.specified_format.def = GET_NEXT_BIT(item, 11);
		}
		if (t->catalog == 0 || t->catalog == 1)
		{
			dst->info_format.specified_format.max_dur = GET_NEXT_BIT(item, 8);
			if (dst->info_format.specified_format.max_dur != 0)
			{
				dst->info_format.specified_format.cur_dur = GET_NEXT_BIT(item, 9);
			}
		}
		if (dst->socketed)
		{
			dst->info_format.specified_format.socket_num = GET_NEXT_BIT(item, 4);
		}
		if (t->stackable)
		{
			dst->info_format.specified_format.quantity = GET_NEXT_BIT(item, 9);
		}
		if (dst->info_format.extended_format.quality == 5)
		{
			for (int i = 0; i < 5; i ++)
			{
				dst->info_format.specified_format.has_set_prop[i] = GET_NEXT_BIT(item, 1);
			}
		}

		if (item_readprop(item, bitpos, &dst->info_format.specified_format.props) == -1)
		{
			return -1;
		}

		for (int i = 0; i < 5; i ++)
		{
			if (dst->info_format.specified_format.has_set_prop[i])
			{
				if (item_readprop(item, bitpos, &dst->info_format.specified_format.set_props[i]) == -1)
				{
					return -1;
				}
			}
		}
		if (dst->rune_word)
		{
			if (item_readprop(item, bitpos, &dst->info_format.specified_format.rw_props) == -1)
			{
				return -1;
			}
		}

	}
	return (bitpos / 8) + ((bitpos % 8 != 0) ? 1 : 0);
}

int get_itemlist_pointer(char* data, int len)
{
	if (len <= 0x2FF) return -1;
	unsigned int i = 0;
	int pos = 0;
	int statfound = 0;
	char* buf = (char*)&data[0x2FF];
	while(pos < 54*8)
	{
		int statid = get_d2_bit(buf, pos, 9);
		pos += 9;
		switch(statid){
		case 0:
			pos += 10;
			break;
		case 1:
			pos += 10;
			break;
		case 2:
			pos += 10;
			break;
		case 3:
			pos += 10;
			break;
		case 4:
			pos += 10;
			break;
		case 5:
			pos += 8;
			break;
		case 6:
			pos += 21;
			break;
		case 7:
			pos += 21;
			break;
		case 8:
			pos += 21;
			break;
		case 9:
			pos += 21;
			break;
		case 10:
			pos += 21;
			break;
		case 11:
			pos += 21;
			break;
		case 12:
			pos += 7;
			break;
		case 13:
			pos += 32;
			break;
		case 14:
			pos += 25;
			break;
		case 15:
			pos += 25;
			break;
		case 511:
			statfound = 1;
			goto stat_found;
		}
	}
stat_found:
	if (statfound)
	{
		i = 0x2FF + (int)ceil((float)pos / 8) + 2 + 30;
	}
	return i;
}

int item_readlist(char* filename, t_d2_item_list* dst)
{
	memset(dst, 0, sizeof(t_d2_item_list));

	FILE *fp;
	fp = fopen(filename, "rb");
	if (!fp) return -1;

	fseek(fp, 0, SEEK_END);
	
	int size = ftell(fp);
	char* data = (char*)malloc(size);
	fseek(fp, 0, SEEK_SET);
	fread(data, 1, size, fp);
	fclose(fp);

	int i = get_itemlist_pointer(data, size);
	if (i < 0) goto cleanup;

	dst->char_flag = data[0x24];

	//read majic
	dst->majic[0] = data[i++];
	dst->majic[1] = data[i++];
	if (dst->majic[0] != 'J' || dst->majic[1] != 'M') goto cleanup;

	//read count
	int item_count = data[i] + (data[i+1] << 8);
	dst->count = item_count;
	i += 2;

	int c = 0;
	int v = 0;
	if (item_count > 0)
	{
		while(i < size)
		{
			if(data[i] == 'J' && data[i+1] == 'M')
			{
				int len = item_raw2struct((char*)&data[i], &dst->items[c]);
				i += len;
				if (dst->items[c].location != 6) v++;
				if (v == item_count && dst->items[c].socketed && dst->items[c].info_format.extended_format.gems > 0)
				{
					int tmp = dst->items[c].info_format.extended_format.gems;
					for (int k = 0; k < tmp; k ++)
					{
						len = item_raw2struct((char*)&data[i], &dst->items[++c]);
						i += len;
					}
				}
				c ++;
				if (v >= item_count) break;
				continue;
			}
			else
			{
				break;
				//we're not supposed to be here
			
			}
			i++;
		}
	}

	dst->real_count = c;

	free(data);
	return 0;
cleanup:
	free(data);
	return -1;
}

int d2s_checksum(unsigned char const * data, unsigned int len,unsigned int offset)
{
	int		checksum;
	unsigned int	i;
	unsigned int	ch;

	if (!data) return 0;
	checksum=0;
	for (i=0; i<len; i++) {
		ch=data[i];
		if (i>=offset && i<offset+sizeof(int)) ch=0;
		ch+=(checksum<0);
		checksum=2*checksum+ch;
	}
	return checksum;
}

int item_overwritelist(char* filename, const t_d2_item_list* items)
{
	FILE *fp;
	fp = fopen(filename, "rb");
	if (!fp) return -1;
	fseek(fp, 0, SEEK_END);
	
	int size = ftell(fp);
	char* data = (char*)malloc(size);
	fseek(fp, 0, SEEK_SET);
	fread(data, 1, size, fp);
	fclose(fp);

	int i = get_itemlist_pointer(data, size);
	if (i < 0) goto cleanup;
	
	int headerlen = i;
	char* header = (char*)malloc(i);
	memcpy(header, data, i);

	//read count
	i += 2;
	int item_count = data[i] + (data[i+1] << 8);
	i += 2;

	t_d2_item_struct tmp;
	int c = 0;
	int v = 0;
	if (item_count > 0)
	{
		while(i < size)
		{
			if(data[i] == 'J' && data[i+1] == 'M')
			{
				int len = item_raw2struct((char*)&data[i], &tmp);
				i += len;
				if (tmp.location != 6) v++;
				if (v == item_count && tmp.socketed && tmp.info_format.extended_format.gems > 0)
				{
					int t = tmp.info_format.extended_format.gems;
					for (int k = 0; k < t; k ++)
					{
						len = item_raw2struct((char*)&data[i], &tmp);
						i += len;
					}
				}
				c ++;
				if (v >= item_count) break;
				continue;
			}
			else
			{
				break;
				//we're not supposed to be here
			
			}
			i++;
		}
	}

	int footerlen = size - i;
	char* footer = (char*)malloc(size - i);
	memcpy(footer, &data[i], size - i);

	free(data);
	
	char d2save[32768];
	int pos = 0;
	memset(d2save, 0, 32768);
	memcpy(&d2save[pos], header, headerlen);
	pos += headerlen;
	memcpy(&d2save[pos], items, 4);
	pos += 4;
	for (int i = 0; i < items->real_count; i ++)
	{
		pos += item_struct2raw(&items->items[i], &d2save[pos]);
	}
	memcpy(&d2save[pos], footer, footerlen);
	pos += footerlen;
	fwrite(footer, 1, footerlen, fp);
	free(header);
	free(footer);

	unsigned int checksum = d2s_checksum((unsigned char*)d2save, pos, 12);
	memcpy(&d2save[0x0C], &checksum, 4);
	memcpy(&d2save[0x08], &pos, 2);

	fp = fopen(filename, "wb");
	if (!fp) goto cleanup;
	fwrite(d2save, 1, pos, fp);
	fclose(fp);

	return 0;
cleanup:
	free(data);
	return -1;
}

int item_getslot(t_d2_item_list *items, int width, int height, t_d2_item_slot *dst)
{
	int stash[STASH_COLUMN][STASH_ROW];
	int inventory[INVENTORY_COLUMN][INVENTORY_ROW];
	memset(stash, 0, sizeof(int) * STASH_COLUMN * STASH_ROW);
	memset(inventory, 0, sizeof(int) * INVENTORY_COLUMN * INVENTORY_ROW);
	for (int i = 0; i < items->real_count; i ++)
	{
		t_d2_item_type *t = item_get_type(items->items[i].info_format.type);
		switch (items->items[i].stored_loc)
		{
		case STORE_LOC_INVENTORY:
			for (int x = items->items[i].col; x < items->items[i].col + t->inv_width; x ++)
			{
				for (int y = items->items[i].row; y < items->items[i].row + t->inv_height; y ++)
				{
					inventory[x][y] = 1;
				}
			}
			break;
		case STORE_LOC_STASH:
			for (int x = items->items[i].col; x < items->items[i].col + t->inv_width; x ++)
			{
				for (int y = items->items[i].row; y < items->items[i].row + t->inv_height; y ++)
				{
					stash[x][y] = 1;
				}
			}
			break;
		}
	}
	int row_size = STASH_ROW;
	if (!(items->char_flag & CHAR_FLAG_EXPANSION)) row_size = STASH_ROW_NONEXP;
	for (int i = 0; i <= STASH_COLUMN - width; i ++)
	{
		for (int j = 0; j <= row_size - height; j ++)
		{
			if (stash[i][j]) goto nxt1;
			for (int x = 0; x < width; x ++)
			{
				for (int y = 0; y < height; y ++)
				{
					if (stash[i+x][j+y]) goto nxt1;
				}
			}
			dst->stored_loc = STORE_LOC_STASH;
			dst->x = i;
			dst->y = j;
			return 1;
nxt1:
			continue;
		}
	}
	for (int i = 0; i <= INVENTORY_COLUMN - width; i ++)
	{
		for (int j = 0; j <= INVENTORY_ROW - height; j ++)
		{
			for (int x = 0; x < width; x ++)
			{
				for (int y = 0; y < height; y ++)
				{
					if (inventory[i+x][j+y]) goto nxt2;
				}
			}
			dst->stored_loc = STORE_LOC_INVENTORY;
			dst->x = i;
			dst->y = j;
			return 1;
nxt2:
			continue;
		}
	}
	return -1;
}

_declspec(dllexport) int item_readcsv(char* filename, int n_normcode, int n_ubercode, int n_ultracode, int n_inv_width, int n_inv_height, int n_stackable, int catalog)
{
	FILE* fp;
	fp = fopen(filename, "r");
	if (!fp) return -1;
	char buf[4096];
	fgets(buf, 4096, fp);
	while (!feof(fp))
	{
		fgets(buf, 4096, fp);
		int len = strlen(buf);
		if (len < 1) continue;
		int last = 0;
		int n = 0;
		char normcode[4];
		char ubercode[4];
		char ultracode[4];
		int inv_height;
		int inv_width;
		int stackable;
		normcode[0] = 0;
		ubercode[0] = 0;
		ultracode[0] = 0;
		for (int i = 0; i <= len; i ++)
		{
			if (buf[i] == '\t' || buf[i] == 0)
			{
				char col[128];
				memcpy(col, &buf[last], i - last);
				col[i - last] = 0;
				if (n == n_normcode)
					strcpy(normcode, col);
				else if (n == n_ubercode)
					strcpy(ubercode, col);
				else if (n == n_ultracode)
					strcpy(ultracode, col);
				else if (n == n_inv_width)
					inv_width = atoi(col);
				else if (n == n_inv_height)
					inv_height = atoi(col);
				else if (n == n_stackable)
					stackable = atoi(col);
				last = i + 1;
				n ++;
			}
		}
		if (normcode[0])
		{
			item_types[item_types_count].catalog = catalog;
			strcpy(item_types[item_types_count].type, normcode);
			item_types[item_types_count].inv_width = inv_width;
			item_types[item_types_count].inv_height = inv_height;
			item_types[item_types_count++].stackable = stackable;
		}
		if (ubercode[0])
		{
			item_types[item_types_count].catalog = catalog;
			strcpy(item_types[item_types_count].type, ubercode);
			item_types[item_types_count].inv_width = inv_width;
			item_types[item_types_count].inv_height = inv_height;
			item_types[item_types_count++].stackable = stackable;
		}
		if (ultracode[0])
		{
			item_types[item_types_count].catalog = catalog;
			strcpy(item_types[item_types_count].type, ultracode);
			item_types[item_types_count].inv_width = inv_width;
			item_types[item_types_count].inv_height = inv_height;
			item_types[item_types_count++].stackable = stackable;
		}
	}
	return item_types_count;
}

int item_fromfile(char* filename, t_d2_item_struct *item)
{
	FILE* fp;
	fp = fopen(filename, "rb");
	if (!fp) return -1;
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char buf[512];
	fread(buf, 1, size, fp);
	fclose(fp);
	int len = item_raw2struct(buf, item);
	if (len == -1) return -1;
	return len;
}

_declspec(dllexport) int item_appendfromfile(char* charsave, char* d2i, int count)
{
	t_d2_item_list items;
	t_d2_item_struct newitem;
	t_d2_item_slot slot;
	t_d2_item_type* type;
	if (item_readlist(charsave, &items) == -1) return -1;
	if (item_fromfile(d2i, &newitem) == -1) return -2;
	type = item_get_type(newitem.info_format.type);
	for (int i = 0; i < count; i ++)
	{
		if (item_getslot(&items, type->inv_width, type->inv_height, &slot) == -1) return -3;
		newitem.stored_loc = slot.stored_loc;
		newitem.col = slot.x;
		newitem.row = slot.y;
		memcpy(&items.items[items.real_count++], &newitem, sizeof(t_d2_item_struct));
	}
	items.count += count;
	if (item_overwritelist(charsave, &items) == -1) return -4;
	return 0;
}

int prop_fromjson(cJSON *json, t_d2_item_prop_list *prop)
{
	prop->count = cJSON_GetArraySize(json);
	for (int i = 0; i < prop->count; i ++)
	{
		cJSON *node = cJSON_GetArrayItem(json, i);
		int code = cJSON_GetObjectItem(node, "code")->valueint;
		cJSON *args = cJSON_GetObjectItem(node, "args");
		prop->props[i].prop_code = code;
		t_d2_prop* propattr = &item_properties[prop->props[i].prop_code];
		for (int j = 0; j < propattr->param_count; j ++)
			prop->props[i].prop_args[j] = cJSON_GetArrayItem(args, j)->valueint;
	}
	return prop->count;
}

cJSON *prop_jsonize(t_d2_item_prop_list *prop)
{
	cJSON *json;
	json = cJSON_CreateArray();
	for (int i = 0; i < prop->count; i ++)
	{
		t_d2_prop* propattr = &item_properties[prop->props[i].prop_code];
		if (propattr == NULL)
		{
			int x = 0;
		}
		cJSON *node;
		node = cJSON_CreateObject();
		cJSON *args;
		args = cJSON_CreateArray();
		cJSON_AddItemToObject(node, "code", cJSON_CreateNumber(prop->props[i].prop_code));
		for (int j = 0; j < propattr->param_count; j ++)
			cJSON_AddItemToArray(args, cJSON_CreateNumber(prop->props[i].prop_args[j]));
		cJSON_AddItemToObject(node, "args", args);
		cJSON_AddItemToArray(json, node);
	}
	return json;
}

cJSON *item_jsonize(t_d2_item_struct *item)
{
	cJSON *json;
	json = cJSON_CreateObject();
	
	cJSON_AddItemToObject(json, "quest_item", cJSON_CreateNumber(item->quest_item));
	cJSON_AddItemToObject(json, "unk21", cJSON_CreateNumber(item->unk21));
	cJSON_AddItemToObject(json, "identified", cJSON_CreateNumber(item->identified));
	cJSON_AddItemToObject(json, "unk25", cJSON_CreateNumber(item->unk25));
	cJSON_AddItemToObject(json, "unk30", cJSON_CreateNumber(item->unk30));
	cJSON_AddItemToObject(json, "duplicated", cJSON_CreateNumber(item->duplicated));
	cJSON_AddItemToObject(json, "socketed", cJSON_CreateNumber(item->socketed));
	cJSON_AddItemToObject(json, "unk34", cJSON_CreateNumber(item->unk34));
	cJSON_AddItemToObject(json, "illegal", cJSON_CreateNumber(item->illegal));
	cJSON_AddItemToObject(json, "unk37", cJSON_CreateNumber(item->unk37));
	cJSON_AddItemToObject(json, "ear", cJSON_CreateNumber(item->ear));
	cJSON_AddItemToObject(json, "starter", cJSON_CreateNumber(item->starter));
	cJSON_AddItemToObject(json, "unk42", cJSON_CreateNumber(item->unk42));
	cJSON_AddItemToObject(json, "unk44", cJSON_CreateNumber(item->unk44));
	cJSON_AddItemToObject(json, "simple", cJSON_CreateNumber(item->simple));
	cJSON_AddItemToObject(json, "ethereal", cJSON_CreateNumber(item->ethereal));
	cJSON_AddItemToObject(json, "unk47", cJSON_CreateNumber(item->unk47));
	cJSON_AddItemToObject(json, "inscribed", cJSON_CreateNumber(item->inscribed));
	cJSON_AddItemToObject(json, "unk51", cJSON_CreateNumber(item->unk51));
	cJSON_AddItemToObject(json, "rune_word", cJSON_CreateNumber(item->rune_word));
	cJSON_AddItemToObject(json, "unk53", cJSON_CreateNumber(item->unk53));
	cJSON_AddItemToObject(json, "unk60", cJSON_CreateNumber(item->unk60));
	cJSON_AddItemToObject(json, "unk70", cJSON_CreateNumber(item->unk70));
	cJSON_AddItemToObject(json, "location", cJSON_CreateNumber(item->location));
	cJSON_AddItemToObject(json, "position", cJSON_CreateNumber(item->position));
	cJSON_AddItemToObject(json, "col", cJSON_CreateNumber(item->col));
	cJSON_AddItemToObject(json, "row", cJSON_CreateNumber(item->row));
	cJSON_AddItemToObject(json, "stored_loc", cJSON_CreateNumber(item->stored_loc));
	char type[4];
	memcpy(type, item->info_format.type, 3);
	type[3] = 0;
	cJSON_AddItemToObject(json, "type", cJSON_CreateString(type));
	cJSON_AddItemToObject(json, "has_rand", cJSON_CreateNumber(item->info_format.has_rand));

	if (item->ear)
	{
		cJSON *ear;
		ear = cJSON_CreateObject();
		cJSON_AddItemToObject(ear, "class", cJSON_CreateNumber(item->ear_format.ear_class));
		cJSON_AddItemToObject(ear, "level", cJSON_CreateNumber(item->ear_format.ear_level));
		cJSON_AddItemToObject(ear, "charname", cJSON_CreateString((char*)item->ear_format.player));
		cJSON_AddItemToObject(json, "ear_format", ear);
	}
	
	if (!item->ear)
	{
		if (!item->simple)
		{
			cJSON *ext;
			ext = cJSON_CreateObject();
			char guid[16];
			guid[0] = '0';
			guid[1] = 'x';
			itoa(item->info_format.extended_format.guid, &guid[2], 16);
			cJSON_AddItemToObject(ext, "gems", cJSON_CreateNumber(item->info_format.extended_format.gems));
			cJSON_AddItemToObject(ext, "guid", cJSON_CreateString(guid));
			cJSON_AddItemToObject(ext, "drop_level", cJSON_CreateNumber(item->info_format.extended_format.drop_level));
			cJSON_AddItemToObject(ext, "quality", cJSON_CreateNumber(item->info_format.extended_format.quality));
			cJSON_AddItemToObject(ext, "vargfx", cJSON_CreateNumber(item->info_format.extended_format.vargfx));
			cJSON_AddItemToObject(ext, "n_vargfx", cJSON_CreateNumber(item->info_format.extended_format.n_vargfx));
			cJSON_AddItemToObject(ext, "has_class", cJSON_CreateNumber(item->info_format.extended_format.has_class));
			cJSON_AddItemToObject(ext, "class_info", cJSON_CreateNumber(item->info_format.extended_format.class_info));
			cJSON_AddItemToObject(ext, "low_quality", cJSON_CreateNumber(item->info_format.extended_format.low_quality));
			cJSON_AddItemToObject(ext, "high_quality", cJSON_CreateNumber(item->info_format.extended_format.high_quality));
			cJSON_AddItemToObject(ext, "magic_prefix", cJSON_CreateNumber(item->info_format.extended_format.magic_prefix));
			cJSON_AddItemToObject(ext, "magic_suffix", cJSON_CreateNumber(item->info_format.extended_format.magic_suffix));
			cJSON_AddItemToObject(ext, "set_id", cJSON_CreateNumber(item->info_format.extended_format.set_id));
			cJSON_AddItemToObject(ext, "rare_name_1", cJSON_CreateNumber(item->info_format.extended_format.rare_name_1));
			cJSON_AddItemToObject(ext, "rare_name_2", cJSON_CreateNumber(item->info_format.extended_format.rare_name_2));
			cJSON_AddItemToObject(ext, "has_prefix_1", cJSON_CreateNumber(item->info_format.extended_format.has_prefix_1));
			cJSON_AddItemToObject(ext, "prefix_1", cJSON_CreateNumber(item->info_format.extended_format.prefix_1));
			cJSON_AddItemToObject(ext, "has_suffix_1", cJSON_CreateNumber(item->info_format.extended_format.has_suffix_1));
			cJSON_AddItemToObject(ext, "suffix_1", cJSON_CreateNumber(item->info_format.extended_format.suffix_1));
			cJSON_AddItemToObject(ext, "has_prefix_2", cJSON_CreateNumber(item->info_format.extended_format.has_prefix_2));
			cJSON_AddItemToObject(ext, "prefix_2", cJSON_CreateNumber(item->info_format.extended_format.prefix_2));
			cJSON_AddItemToObject(ext, "has_suffix_2", cJSON_CreateNumber(item->info_format.extended_format.has_suffix_2));
			cJSON_AddItemToObject(ext, "suffix_2", cJSON_CreateNumber(item->info_format.extended_format.suffix_2));
			cJSON_AddItemToObject(ext, "has_prefix_3", cJSON_CreateNumber(item->info_format.extended_format.has_prefix_3));
			cJSON_AddItemToObject(ext, "prefix_3", cJSON_CreateNumber(item->info_format.extended_format.prefix_3));
			cJSON_AddItemToObject(ext, "has_suffix_3", cJSON_CreateNumber(item->info_format.extended_format.has_suffix_3));
			cJSON_AddItemToObject(ext, "suffix_3", cJSON_CreateNumber(item->info_format.extended_format.suffix_3));
			cJSON_AddItemToObject(ext, "unique_id", cJSON_CreateNumber(item->info_format.extended_format.unique_id));
			cJSON_AddItemToObject(ext, "rune_word", cJSON_CreateNumber(item->info_format.extended_format.rune_word));
			cJSON_AddItemToObject(ext, "inscribed_name", cJSON_CreateString((char*)item->info_format.extended_format.inscribed_name));
			cJSON_AddItemToObject(ext, "spell_id", cJSON_CreateNumber(item->info_format.extended_format.spell_id));
			cJSON_AddItemToObject(json, "extended_format", ext);
		}

		if (!item->simple && !item->ear)
		{
			cJSON *spec;
			spec = cJSON_CreateObject();
			cJSON_AddItemToObject(spec, "def", cJSON_CreateNumber(item->info_format.specified_format.def));
			cJSON_AddItemToObject(spec, "max_dur", cJSON_CreateNumber(item->info_format.specified_format.max_dur));
			cJSON_AddItemToObject(spec, "cur_dur", cJSON_CreateNumber(item->info_format.specified_format.cur_dur));
			cJSON_AddItemToObject(spec, "socket_num", cJSON_CreateNumber(item->info_format.specified_format.socket_num));
			cJSON_AddItemToObject(spec, "quantity", cJSON_CreateNumber(item->info_format.specified_format.quantity));
			cJSON_AddItemToObject(spec, "has_set_prop_1", cJSON_CreateNumber(item->info_format.specified_format.has_set_prop[0]));
			cJSON_AddItemToObject(spec, "has_set_prop_2", cJSON_CreateNumber(item->info_format.specified_format.has_set_prop[1]));
			cJSON_AddItemToObject(spec, "has_set_prop_3", cJSON_CreateNumber(item->info_format.specified_format.has_set_prop[2]));
			cJSON_AddItemToObject(spec, "has_set_prop_4", cJSON_CreateNumber(item->info_format.specified_format.has_set_prop[3]));
			cJSON_AddItemToObject(spec, "has_set_prop_5", cJSON_CreateNumber(item->info_format.specified_format.has_set_prop[4]));
			cJSON_AddItemToObject(spec, "props", prop_jsonize(&item->info_format.specified_format.props));
			cJSON_AddItemToObject(spec, "set_props_1", prop_jsonize(&item->info_format.specified_format.set_props[0]));
			cJSON_AddItemToObject(spec, "set_props_2", prop_jsonize(&item->info_format.specified_format.set_props[1]));
			cJSON_AddItemToObject(spec, "set_props_3", prop_jsonize(&item->info_format.specified_format.set_props[2]));
			cJSON_AddItemToObject(spec, "set_props_4", prop_jsonize(&item->info_format.specified_format.set_props[3]));
			cJSON_AddItemToObject(spec, "set_props_5", prop_jsonize(&item->info_format.specified_format.set_props[4]));
			cJSON_AddItemToObject(spec, "rw_props", prop_jsonize(&item->info_format.specified_format.rw_props));
			cJSON_AddItemToObject(json, "specified_format", spec);
		}

		if (item->info_format.has_rand)
		{
			char pad[16];
			pad[0] = '0';
			pad[1] = 'x';
			itoa(item->info_format.pad1, &pad[2], 16);
			cJSON_AddItemToObject(json, "pad1", cJSON_CreateString(pad));
			itoa(item->info_format.pad2, &pad[2], 16);
			cJSON_AddItemToObject(json, "pad2", cJSON_CreateString(pad));
			itoa(item->info_format.pad3, &pad[2], 16);
			cJSON_AddItemToObject(json, "pad3", cJSON_CreateString(pad));
		}
	}
	return json;
}

int item_jsontoitem(cJSON *json, t_d2_item_struct *item)
{
	item->majic[0] = 'J';
	item->majic[1] = 'M';
	item->quest_item = cJSON_GetObjectItem(json, "quest_item")->valueint;
	item->unk21 = cJSON_GetObjectItem(json, "unk21")->valueint;
	item->identified = cJSON_GetObjectItem(json, "identified")->valueint;
	item->unk25 = cJSON_GetObjectItem(json, "unk25")->valueint;
	item->unk30 = cJSON_GetObjectItem(json, "unk30")->valueint;
	item->duplicated = cJSON_GetObjectItem(json, "duplicated")->valueint;
	item->socketed = cJSON_GetObjectItem(json, "socketed")->valueint;
	item->unk34 = cJSON_GetObjectItem(json, "unk34")->valueint;
	item->illegal = cJSON_GetObjectItem(json, "illegal")->valueint;
	item->unk37 = cJSON_GetObjectItem(json, "unk37")->valueint;
	item->ear = cJSON_GetObjectItem(json, "ear")->valueint;
	item->starter = cJSON_GetObjectItem(json, "starter")->valueint;
	item->unk42 = cJSON_GetObjectItem(json, "unk42")->valueint;
	item->unk44 = cJSON_GetObjectItem(json, "unk44")->valueint;
	item->simple = cJSON_GetObjectItem(json, "simple")->valueint;
	item->ethereal = cJSON_GetObjectItem(json, "ethereal")->valueint;
	item->unk47 = cJSON_GetObjectItem(json, "unk47")->valueint;
	item->inscribed = cJSON_GetObjectItem(json, "inscribed")->valueint;
	item->unk51 = cJSON_GetObjectItem(json, "unk51")->valueint;
	item->rune_word = cJSON_GetObjectItem(json, "rune_word")->valueint;
	item->unk53 = cJSON_GetObjectItem(json, "unk53")->valueint;
	item->unk60 = cJSON_GetObjectItem(json, "unk60")->valueint;
	item->unk70 = cJSON_GetObjectItem(json, "unk70")->valueint;
	item->location = cJSON_GetObjectItem(json, "location")->valueint;
	item->position = cJSON_GetObjectItem(json, "position")->valueint;
	item->col = cJSON_GetObjectItem(json, "col")->valueint;
	item->row = cJSON_GetObjectItem(json, "row")->valueint;
	item->stored_loc = cJSON_GetObjectItem(json, "stored_loc")->valueint;
	char* type = cJSON_GetObjectItem(json, "type")->valuestring;
	memcpy(item->info_format.type, type, 3);
	item->info_format.type[3] = 0x20;
	item->info_format.has_rand = cJSON_GetObjectItem(json, "has_rand")->valueint;

	if (item->ear)
	{
		cJSON *ear = cJSON_GetObjectItem(json, "ear_format");
		item->ear_format.ear_class = cJSON_GetObjectItem(ear, "class")->valueint;
		item->ear_format.ear_level = cJSON_GetObjectItem(ear, "level")->valueint;
		strcpy((char*)item->ear_format.player, cJSON_GetObjectItem(ear, "charname")->valuestring);
	}
	
	if (!item->ear)
	{
		if (!item->simple)
		{
			if (memcmp(type, "buc", 3) == 0)
			{
				int x = 0;
			}
			cJSON *ext = cJSON_GetObjectItem(json, "extended_format");
			item->info_format.extended_format.gems = cJSON_GetObjectItem(ext,  "gems")->valueint;
			item->info_format.extended_format.guid = strtoul(cJSON_GetObjectItem(ext,  "guid")->valuestring, NULL, 16);
			item->info_format.extended_format.drop_level = cJSON_GetObjectItem(ext,  "drop_level")->valueint;
			item->info_format.extended_format.quality = cJSON_GetObjectItem(ext,  "quality")->valueint;
			item->info_format.extended_format.vargfx = cJSON_GetObjectItem(ext,  "vargfx")->valueint;
			item->info_format.extended_format.n_vargfx = cJSON_GetObjectItem(ext,  "n_vargfx")->valueint;
			item->info_format.extended_format.has_class = cJSON_GetObjectItem(ext,  "has_class")->valueint;
			item->info_format.extended_format.class_info = cJSON_GetObjectItem(ext,  "class_info")->valueint;
			item->info_format.extended_format.low_quality = cJSON_GetObjectItem(ext,  "low_quality")->valueint;
			item->info_format.extended_format.high_quality = cJSON_GetObjectItem(ext,  "high_quality")->valueint;
			item->info_format.extended_format.magic_prefix = cJSON_GetObjectItem(ext,  "magic_prefix")->valueint;
			item->info_format.extended_format.magic_suffix = cJSON_GetObjectItem(ext,  "magic_suffix")->valueint;
			item->info_format.extended_format.set_id = cJSON_GetObjectItem(ext,  "set_id")->valueint;
			item->info_format.extended_format.rare_name_1 = cJSON_GetObjectItem(ext,  "rare_name_1")->valueint;
			item->info_format.extended_format.rare_name_2 = cJSON_GetObjectItem(ext,  "rare_name_2")->valueint;
			item->info_format.extended_format.has_prefix_1 = cJSON_GetObjectItem(ext,  "has_prefix_1")->valueint;
			item->info_format.extended_format.prefix_1 = cJSON_GetObjectItem(ext,  "prefix_1")->valueint;
			item->info_format.extended_format.has_suffix_1 = cJSON_GetObjectItem(ext,  "has_suffix_1")->valueint;
			item->info_format.extended_format.suffix_1 = cJSON_GetObjectItem(ext,  "suffix_1")->valueint;
			item->info_format.extended_format.has_prefix_2 = cJSON_GetObjectItem(ext,  "has_prefix_2")->valueint;
			item->info_format.extended_format.prefix_2 = cJSON_GetObjectItem(ext,  "prefix_2")->valueint;
			item->info_format.extended_format.has_suffix_2 = cJSON_GetObjectItem(ext,  "has_suffix_2")->valueint;
			item->info_format.extended_format.suffix_2 = cJSON_GetObjectItem(ext,  "suffix_2")->valueint;
			item->info_format.extended_format.has_prefix_3 = cJSON_GetObjectItem(ext,  "has_prefix_3")->valueint;
			item->info_format.extended_format.prefix_3 = cJSON_GetObjectItem(ext,  "prefix_3")->valueint;
			item->info_format.extended_format.has_suffix_3 = cJSON_GetObjectItem(ext,  "has_suffix_3")->valueint;
			item->info_format.extended_format.suffix_3 = cJSON_GetObjectItem(ext,  "suffix_3")->valueint;
			item->info_format.extended_format.unique_id = cJSON_GetObjectItem(ext,  "unique_id")->valueint;
			item->info_format.extended_format.rune_word = cJSON_GetObjectItem(ext,  "rune_word")->valueint;
			strcpy((char*)item->info_format.extended_format.inscribed_name, cJSON_GetObjectItem(ext,  "inscribed_name")->valuestring);
			item->info_format.extended_format.spell_id = cJSON_GetObjectItem(ext,  "spell_id")->valueint;
		}

		if (!item->simple && !item->ear)
		{
			cJSON *spec = cJSON_GetObjectItem(json, "specified_format");
			item->info_format.specified_format.def = cJSON_GetObjectItem(spec, "def")->valueint;
			item->info_format.specified_format.max_dur = cJSON_GetObjectItem(spec, "max_dur")->valueint;
			item->info_format.specified_format.cur_dur = cJSON_GetObjectItem(spec, "cur_dur")->valueint;
			item->info_format.specified_format.socket_num = cJSON_GetObjectItem(spec, "socket_num")->valueint;
			item->info_format.specified_format.quantity = cJSON_GetObjectItem(spec, "quantity")->valueint;
			item->info_format.specified_format.has_set_prop[0] = cJSON_GetObjectItem(spec, "has_set_prop_1")->valueint;
			item->info_format.specified_format.has_set_prop[1] = cJSON_GetObjectItem(spec, "has_set_prop_2")->valueint;
			item->info_format.specified_format.has_set_prop[2] = cJSON_GetObjectItem(spec, "has_set_prop_3")->valueint;
			item->info_format.specified_format.has_set_prop[3] = cJSON_GetObjectItem(spec, "has_set_prop_4")->valueint;
			item->info_format.specified_format.has_set_prop[4] = cJSON_GetObjectItem(spec, "has_set_prop_5")->valueint;
			prop_fromjson(cJSON_GetObjectItem(spec, "props"), &item->info_format.specified_format.props);
			prop_fromjson(cJSON_GetObjectItem(spec, "set_props_1"), &item->info_format.specified_format.set_props[0]);
			prop_fromjson(cJSON_GetObjectItem(spec, "set_props_2"), &item->info_format.specified_format.set_props[1]);
			prop_fromjson(cJSON_GetObjectItem(spec, "set_props_3"), &item->info_format.specified_format.set_props[2]);
			prop_fromjson(cJSON_GetObjectItem(spec, "set_props_4"), &item->info_format.specified_format.set_props[3]);
			prop_fromjson(cJSON_GetObjectItem(spec, "set_props_5"), &item->info_format.specified_format.set_props[4]);
			prop_fromjson(cJSON_GetObjectItem(spec, "rw_props"), &item->info_format.specified_format.rw_props);
		}

		if (item->info_format.has_rand)
		{
			item->info_format.pad1 = strtoul(cJSON_GetObjectItem(json,  "pad1")->valuestring, NULL, 16);
			item->info_format.pad2 = strtoul(cJSON_GetObjectItem(json,  "pad2")->valuestring, NULL, 16);
			item->info_format.pad3 = strtoul(cJSON_GetObjectItem(json,  "pad3")->valuestring, NULL, 16);
		}
	}
	return 1;
}

int item_jsontoitems(char* json, t_d2_item_list *items)
{
	cJSON *root = cJSON_Parse(json);
	if (!root) return -1;
	memset(items, 0, sizeof(t_d2_item_list));
	items->majic[0] = 'J';
	items->majic[1] = 'M';
	items->count = cJSON_GetObjectItem(root, "count")->valueint;
	items->real_count = cJSON_GetObjectItem(root, "real_count")->valueint;
	items->char_flag = cJSON_GetObjectItem(root, "char_flag")->valueint;
	cJSON *json_items = cJSON_GetObjectItem(root, "item");
	for (int i = 0; i < cJSON_GetArraySize(json_items); i ++)
	{
		cJSON *node = cJSON_GetArrayItem(json_items, i);
		item_jsontoitem(node, &items->items[i]);
	}
	cJSON_Delete(root);
	return items->count;
}

int item_itemstojson(t_d2_item_list *items, char* buffer)
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(items->count));
	cJSON_AddItemToObject(root, "real_count", cJSON_CreateNumber(items->real_count));
	cJSON_AddItemToObject(root, "char_flag", cJSON_CreateNumber(items->char_flag));
	cJSON *itemlist;
	itemlist = cJSON_CreateArray();
	for (int i = 0; i < items->real_count; i ++)
	{
		cJSON_AddItemToArray(itemlist, item_jsonize(&items->items[i]));
	}
	cJSON_AddItemToObject(root, "item", itemlist);
	char* serialize = cJSON_PrintUnformatted(root);
	strcpy(buffer, serialize);
	free(serialize);
	cJSON_Delete(root);
	return strlen(buffer);
}

_declspec(dllexport) int item_jsonlist(char* charsave, char* buffer)
{
	t_d2_item_list items;
	if (item_readlist(charsave, &items) == -1) return -1;
	return item_itemstojson(&items, buffer);
}

_declspec(dllexport) int item_savelistfromjson(char* charsave, char* json)
{
	t_d2_item_list items;
	item_jsontoitems(json, &items);
	if (item_overwritelist(charsave, &items) == -1) return -1;
	return 0;
}

int item_checkunique(t_d2_item_list *items, t_d2_item_struct *item)
{
	t_d2_item_type *t1 = item_get_type(item->info_format.type);
	for (int i = 0; i < items->real_count; i ++)
	{
		t_d2_item_type *t2 = item_get_type(items->items[i].info_format.type);
		if (memcmp(t1->type, t2->type, 3) == 0)
		{
			if (items->items[i].quest_item) return -1;
			if (memcmp(t1->type, "box", 3) == 0) return -1;
			if (!items->items[i].simple && items->items[i].info_format.extended_format.quality == 7 && !item->simple && item->info_format.extended_format.quality == 7)
			{
				if (memcmp(t1->type, "cm1", 3) == 0) return -1;
				if (memcmp(t1->type, "cm2", 3) == 0) return -1;
				if (memcmp(t1->type, "cm3", 3) == 0) return -1;
			}
		}
	}
	return 0;
}

_declspec(dllexport) int item_appendfromjson(char* charsave, char* json)
{
	t_d2_item_list items;
	t_d2_item_struct newitem;
	t_d2_item_slot slot;
	t_d2_item_type* type;
	if (item_readlist(charsave, &items) == -1) return -1;
	cJSON *root = cJSON_Parse(json);
	int count = cJSON_GetArraySize(root);
	for (int i = 0; i < count; i ++)
	{
		cJSON *node = cJSON_GetArrayItem(root, i);
		cJSON *socket = cJSON_GetObjectItem(node, "socket");
		int repeat = cJSON_GetObjectItem(node, "count")->valueint;
		if (!item_jsontoitem(node, &newitem)) return -2;
		type = item_get_type(newitem.info_format.type);
		for (int j = 0; j < repeat; j ++)
		{
			if (item_getslot(&items, type->inv_width, type->inv_height, &slot) == -1) return -3;
			if (item_checkunique(&items, &newitem) == -1) return -5;
			newitem.stored_loc = slot.stored_loc;
			newitem.col = slot.x;
			newitem.row = slot.y;
			newitem.location = 0;
			memcpy(&items.items[items.real_count++], &newitem, sizeof(t_d2_item_struct));
			items.count ++;
			if (socket && cJSON_GetArraySize(socket) > 0)
			{
				for (int k = 0; k < cJSON_GetArraySize(socket); k ++)
				{
					t_d2_item_struct socket_item;
					if (item_jsontoitem(cJSON_GetArrayItem(socket, k), &socket_item) > -1)
					{
						memcpy(&items.items[items.real_count++], &socket_item, sizeof(t_d2_item_struct));
					}
				}
			}
		}
	}
	if (item_overwritelist(charsave, &items) == -1) return -4;
	return 0;
}

_declspec(dllexport) int item_stripbyindex(char* charsave, int index, int socketindex)
{
	t_d2_item_list items;
	t_d2_item_list items_new;
	if (item_readlist(charsave, &items) == -1) return -1;
	memset(&items_new, 0, sizeof(t_d2_item_list));
	items_new.char_flag = items.char_flag;
	memcpy(items_new.majic, items.majic, 2);
	int pos = 0;
	for (int i = 0; i < items.real_count; i ++)
	{
		if (i >= index && i <= socketindex) continue;
		memcpy(&items_new.items[pos++], &items.items[i], sizeof(t_d2_item_struct));
	}
	items_new.count = items.count - 1;
	items_new.real_count = pos;
	if (item_overwritelist(charsave, &items_new) == -1) return -4;
	return 0;
}

_declspec(dllexport) int item_stripmulbyindex(char* charsave, long* index, long* socket_index, int count)
{
	t_d2_item_list items;
	t_d2_item_list items_new;
	if (item_readlist(charsave, &items) == -1) return -1;
	memset(&items_new, 0, sizeof(t_d2_item_list));
	items_new.char_flag = items.char_flag;
	memcpy(items_new.majic, items.majic, 2);
	int pos = 0;
	for (int i = 0; i < items.real_count; i ++)
	{
		for (int j = 0; j < count; j ++)
		{
			if (i >= index[j] && i <= socket_index[j]) goto skip;
		}
		memcpy(&items_new.items[pos++], &items.items[i], sizeof(t_d2_item_struct));
skip:
		continue;
	}
	items_new.count = items.count - count;
	items_new.real_count = pos;
	if (item_overwritelist(charsave, &items_new) == -1) return -4;
	return 0;
}


int main(int argc, char* argv[])
{
	//t_d2_item_type* t;
	item_types_count = 0;
	item_readcsv("armor.txt", 23, 24, 25, 28, 29, 45, 0);
	item_readcsv("weapons.txt", 34, 35, 36, 41, 42, 43, 1);
	item_readcsv("misc.txt", 13, -1, -1, 17, 18, 43, 2);
	xhg_readcsv("item_exchange.csv");
	
	//t = item_get_type("box");
 	//item_appendfromjson("G:\\RESEARCH_AND_PROJECTS\\website\\ah\\dat\\add-json-item_1352302125.70313_1217", "[{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":0,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":0,\"unk60\":101,\"unk70\":0,\"location\":0,\"position\":0,\"col\":9,\"row\":0,\"stored_loc\":1,\"type\":\"cm3\",\"has_rand\":0,\"extended_format\":{\"gems\":0,\"guid\":\"0xfc346017\",\"drop_level\":85,\"quality\":4,\"vargfx\":1,\"n_vargfx\":1,\"has_class\":0,\"class_info\":0,\"low_quality\":0,\"high_quality\":0,\"magic_prefix\":467,\"magic_suffix\":0,\"set_id\":0,\"rare_name_1\":0,\"rare_name_2\":0,\"has_prefix_1\":0,\"prefix_1\":0,\"has_suffix_1\":0,\"suffix_1\":0,\"has_prefix_2\":0,\"prefix_2\":0,\"has_suffix_2\":0,\"suffix_2\":0,\"has_prefix_3\":0,\"prefix_3\":0,\"has_suffix_3\":0,\"suffix_3\":0,\"unique_id\":0,\"rune_word\":0,\"inscribed_name\":\"\",\"spell_id\":0},\"specified_format\":{\"def\":0,\"max_dur\":0,\"cur_dur\":0,\"socket_num\":0,\"quantity\":0,\"has_set_prop_1\":0,\"has_set_prop_2\":0,\"has_set_prop_3\":0,\"has_set_prop_4\":0,\"has_set_prop_5\":0,\"props\":[{\"code\":188,\"args\":[1,3,1]}],\"set_props_1\":[],\"set_props_2\":[],\"set_props_3\":[],\"set_props_4\":[],\"set_props_5\":[],\"rw_props\":[]},\"socket_index\":8,\"socket\":[],\"count\":1}]");
	//item_appendfromjson("G:\\GAMES\\Diablo_II_LoD\\13\\Save\\fr_lyg.d2s", "[{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":0,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":0,\"unk60\":101,\"unk70\":0,\"location\":0,\"position\":0,\"col\":9,\"row\":2,\"stored_loc\":1,\"type\":\"rin\",\"has_rand\":0,\"extended_format\":{\"gems\":0,\"guid\":\"0xc40b7e59\",\"drop_level\":88,\"quality\":7,\"vargfx\":1,\"n_vargfx\":2,\"has_class\":0,\"class_info\":0,\"low_quality\":0,\"high_quality\":0,\"magic_prefix\":0,\"magic_suffix\":0,\"set_id\":0,\"rare_name_1\":0,\"rare_name_2\":0,\"has_prefix_1\":0,\"prefix_1\":0,\"has_suffix_1\":0,\"suffix_1\":0,\"has_prefix_2\":0,\"prefix_2\":0,\"has_suffix_2\":0,\"suffix_2\":0,\"has_prefix_3\":0,\"prefix_3\":0,\"has_suffix_3\":0,\"suffix_3\":0,\"unique_id\":120,\"rune_word\":0,\"inscribed_name\":\"\",\"spell_id\":0},\"specified_format\":{\"def\":0,\"max_dur\":0,\"cur_dur\":0,\"socket_num\":0,\"quantity\":0,\"has_set_prop_1\":0,\"has_set_prop_2\":0,\"has_set_prop_3\":0,\"has_set_prop_4\":0,\"has_set_prop_5\":0,\"props\":[{\"code\":19,\"args\":[52]},{\"code\":35,\"args\":[3]},{\"code\":78,\"args\":[3]},{\"code\":80,\"args\":[130]}],\"set_props_1\":[],\"set_props_2\":[],\"set_props_3\":[],\"set_props_4\":[],\"set_props_5\":[],\"rw_props\":[]},\"socket_index\":22,\"socket\":[],\"count\":1}]");
	//item_appendfromjson("I:\\tew2.d2s", "[{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":1,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":0,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":1,\"unk53\":1,\"unk60\":101,\"unk70\":0,\"location\":0,\"position\":0,\"col\":0,\"row\":0,\"stored_loc\":5,\"type\":\"uit\",\"has_rand\":0,\"extended_format\":{\"guid\":\"0xb6d74890\",\"drop_level\":88,\"quality\":3,\"vargfx\":0,\"n_vargfx\":0,\"has_class\":0,\"class_info\":0,\"low_quality\":0,\"high_quality\":2,\"magic_prefix\":0,\"magic_suffix\":0,\"set_id\":0,\"rare_name_1\":0,\"rare_name_2\":0,\"has_prefix_1\":0,\"prefix_1\":0,\"has_suffix_1\":0,\"suffix_1\":0,\"has_prefix_2\":0,\"prefix_2\":0,\"has_suffix_2\":0,\"suffix_2\":0,\"has_prefix_3\":0,\"prefix_3\":0,\"has_suffix_3\":0,\"suffix_3\":0,\"unique_id\":0,\"rune_word\":20608,\"inscribed_name\":\"\",\"spell_id\":0},\"specified_format\":{\"def\":159,\"max_dur\":86,\"cur_dur\":84,\"socket_num\":4,\"quantity\":0,\"has_set_prop_1\":0,\"has_set_prop_2\":0,\"has_set_prop_3\":0,\"has_set_prop_4\":0,\"has_set_prop_5\":0,\"props\":[{\"code\":16,\"args\":[15]}],\"set_props_1\":[],\"set_props_2\":[],\"set_props_3\":[],\"set_props_4\":[],\"set_props_5\":[],\"rw_props\":[{\"code\":17,\"args\":[356,356]},{\"code\":32,\"args\":[350]},{\"code\":143,\"args\":[21]},{\"code\":151,\"args\":[124,12]},{\"code\":198,\"args\":[22,225,40]},{\"code\":199,\"args\":[40,46,100]},{\"code\":333,\"args\":[28]}]},\"socket_index\":56,\"socket\":[{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":1,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":1,\"unk60\":101,\"unk70\":0,\"location\":6,\"position\":0,\"col\":0,\"row\":0,\"stored_loc\":0,\"type\":\"r26\",\"has_rand\":0},{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":1,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":1,\"unk60\":101,\"unk70\":0,\"location\":6,\"position\":0,\"col\":1,\"row\":0,\"stored_loc\":0,\"type\":\"r26\",\"has_rand\":0},{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":1,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":1,\"unk60\":101,\"unk70\":0,\"location\":6,\"position\":0,\"col\":2,\"row\":0,\"stored_loc\":0,\"type\":\"r28\",\"has_rand\":0},{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":1,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":1,\"unk60\":101,\"unk70\":0,\"location\":6,\"position\":0,\"col\":3,\"row\":0,\"stored_loc\":0,\"type\":\"r31\",\"has_rand\":0}],\"count\":1}]");
	
	//t_d2_item_list items;
	item_stripbyindex("G:\\RESEARCH_AND_PROJECTS\\website\\ah\\dat\\strip-item_1352557061.45662_793", 63, 64);
	//t_d2_item_list items2;
	//item_readlist("G:\\RESEARCH_AND_PROJECTS\\website\\ah\\dat\\skaara_tt.d2s", &items); //G:\\GAMES\\Diablo_II_LoD\\Diablo 111\\Save\\hd_erqiu.d2s", &items);
	/*long a[] = {70};
	long b[] = {70};
	item_stripmulbyindex("G:\\RESEARCH_AND_PROJECTS\\our_pvpgn\\var\\charsave\\mm-smite", a, b, 1);
	//t_d2_item_list items;
	//item_readlist("G:\\RESEARCH_AND_PROJECTS\\our_pvpgn\\var\\charsave\\mm-smite", &items);
	//item_appendfromjson("G:\\RESEARCH_AND_PROJECTS\\our_pvpgn\\var\\charsave\\mm-smite", "[{\"quest_item\":0,\"unk21\":0,\"identified\":1,\"unk25\":0,\"unk30\":0,\"duplicated\":0,\"socketed\":0,\"unk34\":0,\"illegal\":0,\"unk37\":0,\"ear\":0,\"starter\":0,\"unk42\":0,\"unk44\":0,\"simple\":1,\"ethereal\":0,\"unk47\":1,\"inscribed\":0,\"unk51\":0,\"rune_word\":0,\"unk53\":0,\"unk60\":101,\"unk70\":0,\"location\":0,\"position\":0,\"col\":5,\"row\":7,\"stored_loc\":5,\"type\":\"r08\",\"has_rand\":0,\"socket\":[],\"count\":1}]");
	//xhg_xchg("G:\\vdddd.d2s");
	//t_d2_item_list items2;
	//t_d2_item_slot slot;
	//item_readlist("G:\\vvvvvvv.d2s", &items);
	/*char *buf = (char*)malloc(524288);
	int ret = item_jsonlist("G:\\GAMES\\Diablo_II_LoD\\Diablo 111\\Save\\wxsor-sire.d2s", buf);
	FILE* fp = fopen("E:\\items.json", "wb");
	fwrite(buf, 1, strlen(buf), fp);
	fclose(fp);*/
	/*int len = item_jsontoitems(buf, &items2);

	FILE *fp = fopen("E:\\items.raw", "wb");
	fwrite(&items, 1, sizeof(t_d2_item_list), fp);
	fclose(fp);
	
	fp = fopen("E:\\items2.raw", "wb");
	fwrite(&items2, 1, sizeof(t_d2_item_list), fp);
	fclose(fp);


	int ret = memcmp(&items, &items2, sizeof(t_d2_item_list));
	printf("%d\n",ret);
	
	//int ret = item_getslot(&items, 2, 1, &slot);
	//modify item here
	//item_overwritelist("G:\\wxsor-sire2", &items);*/
}