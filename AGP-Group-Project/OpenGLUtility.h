#pragma once
#ifndef B00289996B00227422_UTILITY_H
#define B00289996B00227422_UTILITY_H

#define VERTEX_POSITION		0
#define VERTEX_NORMAL		1
#define VERTEX_TANGENT		2
#define VERTEX_BITANGENT	3
#define VERTEX_TEXCOORD		4
#define VERTEX_COLOUR		5
#define VERTEX_MAX_BUFFERS	6
#include <string>
#include <GL\glew.h>
class OpenGLUtility {
public:
	~OpenGLUtility() {}
	static std::string GetValueString(GLenum val);
private:

};

#endif // !B00289996B00227422_UTILITY_H
