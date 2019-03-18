#ifndef GLOBAL_SETTINGS_H_
#define GLOBAL_SETTINGS_H_

#include <GLM\glm.hpp>
#include <map>

// This class contains all the global data for settings. It's a singleton which
// means that it should only be one of it hence the get instance function.
// Example:
//		GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_X");
//   

//---------------------------------------------------------
//------------------------ENUMS----------------------------
//---------------------------------------------------------

enum AnimationState {
	ANIMATION_STATE_PLAYER_IDLE,
	ANIMATION_STATE_PLAYER_JUMP,
	ANIMATION_STATE_PLAYER_WALK,
	ANIMATION_STATE_IS_ATTACKING
};

//ObjectIDs------------------------------------------------
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,				//:The Error Model
	OBJECT_ID_PLAYER_IDLE,		//:Start of Player
	OBJECT_ID_PLAYER_JUMP,
	OBJECT_ID_PLAYER_LEFT_WALK_1,
	OBJECT_ID_PLAYER_LEFT_WALK_2,
	OBJECT_ID_PLAYER_LEFT_WALK_3,
	OBJECT_ID_PLAYER_RIGHT_WALK_1,
	OBJECT_ID_PLAYER_RIGHT_WALK_2,
	OBJECT_ID_PLAYER_RIGHT_WALK_3,
	OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_1,
	OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_2,
	OBJECT_ID_PLAYER_ATTACK_SWORD_STANCE_3,
	OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_1,
	OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_2,
	OBJECT_ID_PLAYER_ATTACK_AXE_STANCE_3,
	OBJECT_ID_DUMMY,			//:Start of NPCs
	OBJECT_ID_ICE_AI,
	OBJECT_ID_FIRE_AI,
	OBJECT_ID_WOOD_AI,
	OBJECT_ID_DROP_HP_RESTORE,	//:Start of Drops
	OBJECT_ID_DROP_HP_UP,
	OBJECT_ID_DROP_ATK_UP,
	OBJECT_ID_DROP_SPD_UP,
	OBJECT_ID_DROP_DASH,
	OBJECT_ID_DROP_DOUBLE_JUMP,
	OBJECT_ID_DROP_SWORD,
	OBJECT_ID_DROP_AXE,
	OBJECT_ID_DROP_KEY,
	OBJECT_ID_DROP_DOOR,
	OBJECT_ID_DROP_CHEST_CLOSED,
	OBJECT_ID_DROP_CHEST_OPEN,
	OBJECT_ID_BOSS,				//:Start of Boss
	OBJECT_ID_BOSS_HAND,
	NUMBER_OF_OBJECT_IDS		//:The Last Enum
};

//Ability & Weapon IDs-------------------------------------
enum AbilityID {
	ABILITY_NONE,
	ABILITY_DOUBLE_JUMP,
	ABILITY_DASH,
	NUM_OF_ABILITIES
};

enum WeaponID {
	WEAPON_NONE,
	WEAPON_SWORD,
	WEAPON_AXE,
	NUM_OF_WEAPONS

};

//Zone IDs-------------------------------------------------
enum ZoneID {
	DEF,
	RED,
	GRE,
	BLU,
	NUM_OF_ZONES
};

//States---------------------------------------------------
enum GameState {
	GAME,
	MENU,
	PAUSE,
	OPTIONS,
	DEATH,
	BOSS,
	QUIT, 
	VICTORY
};

//---------------------------------------------------------
//------------------------STRUCTS--------------------------
//---------------------------------------------------------
//Package Structs------------------------------------------
struct ObjectPackage {
	ObjectID id;
	glm::mat4 model_matrix;
};

struct PlayerInfoPackage {
	glm::vec3 position;
	int max_hp;
	int current_hp;
	AbilityID ability_id;
	WeaponID weapon_id;
	int num_of_keys;
};


class GlobalSettings
{
public:
	static GlobalSettings* Access() {
		static GlobalSettings instance;
		return &instance;
	}
	void UpdateValuesFromFile();
	float ValueOf(std::string setting_name);

private:
	GlobalSettings() {};
	~GlobalSettings();

	// Should only be one of this class, this is to make sure nothing ever tries to copy it.
	GlobalSettings(GlobalSettings&) = delete;
	void operator=(GlobalSettings const&) = delete;

	std::map<std::string, float> settings_;
};

#endif