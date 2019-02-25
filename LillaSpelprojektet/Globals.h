#ifndef GLOBALS_H
#define GLOBALS_H

#include "GLM/glm.hpp"

//const int kWindow_Width = 1280;
//const int kWindow_Height = 720;

//Window---------------------------------------------------
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Camera---------------------------------------------------
#define CAMERA_DEFAULT_ZOOM 70.0
#define CAMERA_DEBUG_ZOOM 100.0
#define CAMERA_DEBUG_POSITION_X 256.0
#define CAMERA_DEBUG_POSITION_Y -256.0
#define CAMERA_DEBUG_POSITION_Z 100.0

//Objects--------------------------------------------------
#define OBJECT_CULLING_DISTANCE 1380	//Distance beyond which objects are not updated nor rendered

#define OBJECT_TURN_RATE 45 //degrees	//Purely for the turn function in ObjectClass as of this writing

#define GRAVITATIONAL_ACCELERATION -500	//-g on y-axis	//Affects fall speed
#define OBJECT_MAX_VELOCITY 500
#define OBJECT_MIN_VELOCITY 0.1			//The lowest velocity an object can have that isn't 0
										//If an velocity is lower than this it sets to 0
#define OBJECT_GROUND_LOSS_RATIO 20.0	//0.1 = 10%		//WHAT.
#define OBJECT_AIR_LOSS_RATIO 1.00

//Player object--------------------------------------------
//These numbers are specific to the player. Consider using these as base input for the
//constructor and then altering internal variables
#define PLAYER_TOP_SPEED 100
#define PLAYER_JUMP_VELOCITY 75

//Map------------------------------------------------------
#define GRID_COLUMN 32
#define GRID_ROW 32
#define GRID_GAP 2.0					//2.0 to close the gaps between the cells
#define MAP_DEPTH 0.05	
#define MAP_SIZE 2048

//#define MAP_SCALE 1						//1 -> each vertex have 1 position unit in between
//Ability Numbers------------------------------------------
#define DASH_COOLDOWN 5
#define DASH_VELOCITY 400

//NOTE:
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,				//0:
	OBJECT_ID_PLAYER,			//1:
	OBJECT_ID_JOHNNY_BRAVO,		//2:
	OBJECT_ID_MAP				//3
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

#endif // !GLOBALS_H