#include "OpenGLUtility.h"

std::string OpenGLUtility::GetValueString(GLenum val) {
	switch(val) {
	case GL_INT:
		return "Int";
		break;
	case GL_INT_VEC2:
		return "IntVector2";
		break;
	case GL_INT_VEC3:
		return "IntVector3";
		break;
	case GL_INT_VEC4:
		return "IntVector4";
		break;
	case GL_FLOAT:
		return "Float";
		break;
	case GL_FLOAT_VEC2:
		return "FloatVector2";
		break;
	case GL_FLOAT_VEC3:
		return "FloatVector3";
		break;
	case GL_FLOAT_VEC4:
		return "FloatVector4";
		break;
	case GL_BOOL:
		return "Boolean";
		break;
	case GL_BOOL_VEC2:
		return "BooleanVector2";
		break;
	case GL_BOOL_VEC3:
		return "BooleanVector3";
		break;
	case GL_BOOL_VEC4:
		return "BooleanVector4";
		break;
	case GL_FLOAT_MAT2:
		return "Matrix2";
		break;
	case GL_FLOAT_MAT3:
		return "Matrix3";
		break;
	case GL_FLOAT_MAT4:
		return "Matrix4";
		break;
	case GL_SAMPLER_1D:
		return "Sampler1";
		break;
	case GL_SAMPLER_2D:
		return "Sampler2";
		break;
	case GL_SAMPLER_CUBE:
		return "SamplerCube";
		break;
	default:
		return "Unidentified Value";
		break;
	}
	return std::string();
}
