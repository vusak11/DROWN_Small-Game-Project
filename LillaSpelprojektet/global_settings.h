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

};

#endif