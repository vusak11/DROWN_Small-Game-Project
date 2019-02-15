#ifndef GLOBALS_H
#define GLOBALS_H

#include "GLM/glm.hpp"

//const int kWindow_Width = 1280;
//const int kWindow_Height = 720;

//Window---------------------------------------------------
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

//Objects--------------------------------------------------
#define OBJECT_CULLING_DISTANCE 1380
#define OBJECT_TURN_RATE 45 //degrees
#define GRAVITATIONAL_ACCELERATION -10	//-g on y-axis
#define OBJECT_MAX_VELOCITY 100
#define OBJECT_MIN_VELOCITY 0.1			//The lowest velocity an object can have that isn't 0
										//If an velocity is lower than this it sets to 0
#define OBJECT_DECCELERATION 0.1		//0.1 = 10%



//NOTE:
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,				//0:
	OBJECT_ID_PLAYER,			//1:
	OBJECT_ID_JOHNNY_BRAVO,		//2:
	OBJECT_ID_MAP			//3
};

//Package Structs------------------------------------------
struct ObjectPackage {
	ObjectID id;
	glm::mat4 model_matrix;
};

struct PlayerInfoPackage {
	int hp;
};

//States---------------------------------------------------
enum GameState {
	GAME,
	MENU,
	PAUSE
};

#endif // !GLOBALS_H