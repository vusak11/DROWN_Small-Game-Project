#ifndef GLOBAL_SETTINGS_H_
#define GLOBAL_SETTINGS_H_

#include <GLM\glm.hpp>
#include <map>

// This class contains all the global data for settings. It's a singleton which
// means that it should only be one of it hence the get instance function.
// Example:
//		GlobalSettings::Access()->ValueOf("CAMERA_DEBUG_POSITION_X");
//   

//NOTE:
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,				//0:	The Error Model
	OBJECT_ID_PLACEHOLDER,		//1:
	OBJECT_ID_DUMMY,			//2:
	OBJECT_ID_PLAYER,
	NUMBER_OF_OBJECT_IDS		//N:	The Last Enum
};

//Package Structs------------------------------------------
struct ObjectPackage {
	ObjectID id;
	glm::mat4 model_matrix;
};

struct PlayerInfoPackage {
	int max_hp;
	int current_hp;
	//ability1
	//ability2
};

//States---------------------------------------------------
enum GameState {
	GAME,
	MENU,
	PAUSE,
	OPTIONS,
	DEATH
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

//	//Window---------------------------------------------------
	//int window_width_;
	//int window_height_;
//
////Camera---------------------------------------------------
//	float camera_default_zoom_;
//	float camera_debug_zoom_;
//	float camera_debug_position_x_;
//	float camera_debug_position_y;
//	float camera_debug_position_z_;
//
////Objects--------------------------------------------------
//	int object_culling_distance_;		//Distance beyond which objects are not updated nor rendered
//
//	double PI_;
//	int object_turn_rate_;//degrees/sec	//Purely for the turn function in ObjectClass as of this writing
//
//	float gravitational_acceleration_;	//-g on y-axis	//Affects fall speed
//	float object_max_velocity_;
//	float object_min_velocity_;			//The lowest velocity an object can have that isn't 0
//										//If an velocity is lower than this it sets to 0
//	float object_ground_loss_ratio_;	//0.1 = 10%		//WHAT.
//	float object_air_loss_ratio_;
//
////Player object--------------------------------------------
////These numbers are specific to the player. Consider using these as base input for the
////constructor and then altering internal variables
//	int player_top_speed_;
//	int player_jump_velocity_;
//
////Map------------------------------------------------------
//	unsigned int grid_column_;
//	unsigned int grid_row_;
//	float grid_gap_; 					//2.0 to close the gaps between the cells
//	float map_depth;
//	int map_size;
//
////#define MAP_SCALE 1						//1 -> each vertex have 1 position unit in between
////Ability Numbers------------------------------------------
//	float dash_cooldown;
//	int dash_velocity;
};

#endif