#ifndef  B00289996_CHECK_ERROR_H
#define  B00289996_CHECK_ERROR_H

#include <GL\glew.h>
#include <string>
#include <sstream>
#include <iostream>

namespace B00289996 {
	
	void CheckForGLError(const char *file, int line) {
		GLenum err = glGetError();	
		if(err != GL_NO_ERROR) {
			std::stringstream msg;
			std::string tab = "     ";
			msg << "Errors in " << file << "at line " << line << std::endl;
			do {

				switch(err) {
				case GL_INVALID_ENUM:
					msg << tab << "Invalid Enumeration" << std::endl;
					break;

				case GL_INVALID_VALUE:
					msg << tab << "Invalid Numeric Value (out of range?)" << std::endl;
					break;

				case GL_INVALID_OPERATION:
					msg << tab << "Invalid Operation in Current OpenGL State" << std::endl;
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					msg << tab << "Framebuffer is Incomplete" << std::endl;
					break;

				case GL_STACK_OVERFLOW:
					msg << tab << "Stack Overflow" << std::endl;
					break;

				case GL_STACK_UNDERFLOW:
					msg << tab << "Stack Underflow" << std::endl;
					break;

				case GL_OUT_OF_MEMORY:
					msg << tab << "OpenGL Ran Out of Memory" << std::endl;
					break;

				default:
					break;
				}
				std::cout << msg.str() << std::endl;
				err = glGetError();
			}
			while(err != GL_NO_ERROR);
		}

	}
	#define CheckForOpenGLError() CheckForGLError(__FILE__, __LINE__) 
}

#endif // !B00289996_CHECK_ERROR_H
