//---------------------------------------------------------
//							NOTE:
//			This file contains copy pasted code
//---------------------------------------------------------


#ifndef GLDEBUG_H
#define GLDEBUG_H

#include "GL/glew.h"	//For GL datatypes
#include <iostream>		//For output

#define GL_DEBUG_LAYER

//Call this function to activate debug in OpenGL
//
//Source: Provided by Stefan Petersson & Fransisco Lopez Luro

void EnableGLDebug();

//This function is called by OpenGL's debug message callback
//as set in 'EnableGLDebug()'
//
//Source: Provided by Stefan Petersson & Fransisco Lopez Luro

//#define APIENTRY __stdcall
//void APIENTRY glDebugOutput(
void __stdcall glDebugOutput(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam
);

/*

//This function prints OpenGL errors that are "active" at
//the point in the code it is called.
//Note that OpenGL places errors in a queue, so it might
//display an error that has occured at an earlier point in
//the code compared to the function call
//
//Source: http://www.lighthouse3d.com/cg-topics/error-tracking-in-opengl/
//
//	"Returns 1 if an OpenGL error occurred, 0 otherwise."

#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(char *file, int line);

*/



#endif // !GLDEBUG_H
