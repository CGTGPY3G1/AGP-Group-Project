#pragma once
#ifndef B00289996B00227422_SHADER_PROGRAM_H
#define B00289996B00227422_SHADER_PROGRAM_H
#include <GL\glew.h>
#include <vector>
#include <memory>
namespace B00289996B00227422 {
	struct ShaderValue {
		std::string name;
		GLenum type;
	};
	class ShaderProgram : std::enable_shared_from_this<ShaderProgram> {
		friend class FileLoader;
	public:
		ShaderProgram();
		~ShaderProgram();
		void Init(const std::string & vert, const std::string & frag);
		const GLint GetAttributeLocation(const GLchar *name) const;
		const void BindAttributeLocation(GLuint index, const GLchar *name) const;
		void Link();
		const void Bind() const;
		const void UnBind() const;
		const GLuint GetUniformLocation(const char* name) const;
		template <typename Type> void SetUniform(const char* uniformName, const Type & data) const;
		const GLuint GetID() const;
		template <typename Type> void GetUniformValue(const char* uniformName, Type & data) const;
		std::vector<ShaderValue> GetAttributes();
		std::vector<ShaderValue> GetUniforms();
		const bool operator ==(const std::shared_ptr<ShaderProgram> & other) const;
	private:
		bool valid = false;
		static GLuint currentShader;
		GLuint id;
		std::vector<GLuint> shaderIDs;
	};
}

#endif // !B00289996B00227422_SHADER_PROGRAM_H
