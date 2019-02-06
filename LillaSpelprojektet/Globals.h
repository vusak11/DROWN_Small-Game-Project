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

#define OBJECT_MAX_VELOCITY 100
#define OBJECT_TURN_RATE 45 //degrees

//NOTE:
//Object ID:s are used to determine what type of object is created
//and what model is loaded for an object.
//Anytime a new object or model is added to the game, ensure it is
//represented by an ObjectID
enum ObjectID {
	OBJECT_ID_NULL,			//0:
	OBJECT_ID_PLAYER,			//1:
	OBJECT_ID_JOHNNY_BRAVO		//2:
};

//Package Structs------------------------------------------
struct ObjectPackage {
	ObjectID id;
	glm::mat4 model_matrix;
};

#endif // !GLOBALS_H