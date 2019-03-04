#ifndef GLOBALS_H
#define GLOBALS_H

#include "GLM/glm.hpp"

//---------------------------------------------------------
//------------------------MACROS---------------------------
//---------------------------------------------------------

//Window---------------------------------------------------
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Camera---------------------------------------------------
#define CAMERA_DEFAULT_ZOOM 40.0
#define CAMERA_DEBUG_ZOOM 100.0
#define CAMERA_DEBUG_POSITION_X 256.0
#define CAMERA_DEBUG_POSITION_Y -256.0
#define CAMERA_DEBUG_POSITION_Z 100.0

//Objects--------------------------------------------------
#define OBJECT_CULLING_DISTANCE 100	//Distance beyond which objects are not updated nor rendered

#define PI 3.1415
#define OBJECT_TURN_RATE 720 //degrees/sec	//Purely for the turn function in ObjectClass as of this writing

#define GRAVITATIONAL_ACCELERATION -500	//-g on y-axis	//Affects fall speed
#define OBJECT_MAX_VELOCITY 1000
#define OBJECT_MIN_VELOCITY 0.1			//The lowest velocity an object can have that isn't 0
										//If an velocity is lower than this it sets to 0
#define OBJECT_GROUND_LOSS_RATIO 10.0	//0.1 = 10%		//WHAT.
#define OBJECT_AIR_LOSS_RATIO 1.00

//Player object--------------------------------------------
#define PLAYER_START_POS glm::vec3(224.0f, -160.0f, 0.0f)
#define PLAYER_MOVE_VELOCITY 100
#define PLAYER_MOVE_ACCELERATION (PLAYER_MOVE_VELOCITY/5) //Player accelerates 1/X of its speed per timeunit
#define PLAYER_JUMP_VELOCITY 150
#define PLAYER_START_HP 100
#define PLAYER_START_ATK 100

//Ability Numbers------------------------------------------
#define DASH_COOLDOWN 1
#define DASH_VELOCITY 800

//Drop Numbers---------------------------------------------
#define DROP_HP_RESTORE 10
#define DROP_HP_UP 20
#define DROP_ATK_UP 5

//Map------------------------------------------------------
#define GRID_COLUMN 8
#define GRID_ROW 8
#define GRID_GAP 2.0					//2.0 to close the gaps between the cells
#define MAP_DEPTH 0.05	
#define MAP_SIZE 512
//#define MAP_SCALE 1						//1 -> each vertex have 1 position unit in between

//---------------------------------------------------------
//------------------------ENUMS----------------------------
//---------------------------------------------------------

//NOTE:
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,				//:The Error Model
	OBJECT_ID_PLAYER,			//:Player
	OBJECT_ID_DUMMY,			//:Start of NPCs
	OBJECT_ID_DROP_HP_RESTORE,	//:Start of Drops
	OBJECT_ID_DROP_HP_UP,
	OBJECT_ID_DROP_ATK_UP,
	OBJECT_ID_DROP_DASH,
	OBJECT_ID_DROP_DOUBLE_JUMP,
	OBJECT_ID_DROP_SWORD,		
	OBJECT_ID_DROP_AXE,
	OBJECT_ID_DROP_KEY,
	NUMBER_OF_OBJECT_IDS		//:The Last Enum
};

enum AbilityID {
	ABILITY_NONE,
	ABILITY_DOUBLE_JUMP,
	ABILITY_DASH
};

enum WeaponID {
	WEAPON_SWORD
};

//States---------------------------------------------------
enum GameState {
	GAME,
	MENU,
	PAUSE,
	OPTIONS,
	DEATH,
	QUIT
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
};



#endif // !GLOBALS_H