#ifndef GLOBALS_H
#define GLOBALS_H

#include "GLM/glm.hpp"

//const int kWindow_Width = 1280;
//const int kWindow_Height = 720;

//Window---------------------------------------------------
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Objects--------------------------------------------------
#define OBJECT_CULLING_DISTANCE 1380	//Distance beyond which objects are not updated nor rendered

#define PI 3.1415
#define OBJECT_TURN_RATE 10 //degrees	//Purely for the turn function in ObjectClass as of this writing

#define GRAVITATIONAL_ACCELERATION -500	//-g on y-axis	//Affects fall speed
#define OBJECT_MAX_VELOCITY 500
#define OBJECT_MIN_VELOCITY 0.1			//The lowest velocity an object can have that isn't 0
										//If an velocity is lower than this it sets to 0
#define OBJECT_GROUND_LOSS_RATIO 1.95	//0.1 = 10%		//WHAT.
#define OBJECT_AIR_LOSS_RATIO 1.00

//Player object--------------------------------------------
//These numbers are specific to the player. Consider using these as base input for the
//constructor and then altering internal variables
#define PLAYER_TOP_SPEED 100
#define PLAYER_JUMP_VELOCITY 300

//Ability Numbers------------------------------------------
#define DASH_COOLDOWN 5
#define DASH_VELOCITY 400

//Map------------------------------------------------------
#define MAP_SCALE 5						//1 -> each vertex have 1 position unit in between
#define MAP_DEPTH 0.2					//The depth of the heightmap

//NOTE:
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,				//0:	The Error Model
	OBJECT_ID_PLAYER,			//1:
	OBJECT_ID_DUMMY,			//2:
	NUMBER_OF_OBJECT_IDS		//N:	The Last Enum
};

//Package Structs------------------------------------------
struct ObjectPackage {
	ObjectID id;
	glm::mat4 model_matrix;
};

#endif // !GLOBALS_H