#include "Shader.h"
#include <iostream>
#include <glm\mat4x4.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "gl_error.hpp"
namespace B00289996B00227422 {
	GLuint ShaderProgram::currentShader = 0;

	ShaderProgram::ShaderProgram() {
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(id);
	}

	void ShaderProgram::Init(const std::string & vert, const std::string & frag) {
	}

	const GLint ShaderProgram::GetAttributeLocation(const GLchar * name) const {
		return glGetAttribLocation(id, name);
	}

	const void ShaderProgram::BindAttributeLocation(GLuint index, const GLchar * name) const {
		glBindAttribLocation(id, index, name);
	}

	void ShaderProgram::Link() {
		int isLinked = -1;
		glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
		if(isLinked != GL_FALSE) std::cout << "Already Linked" << std::endl;
		else {
			glLinkProgram(id);
			glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
			if(isLinked == GL_FALSE) std::cout << "Failed Linking Shader" << std::endl;
			for(std::vector<GLuint>::iterator i = shaderIDs.begin(); i != shaderIDs.end(); ++i) glDetachShader(id, (*i));
		}
	}

	const void ShaderProgram::Bind() const {
		if(id != currentShader) {
			glUseProgram(id);
			currentShader = id;
		}
	}

	const void ShaderProgram::UnBind() const {
		glUseProgram(0);
		currentShader = 0;
	}

	const GLuint ShaderProgram::GetUniformLocation(const char * name) const {
		return glGetUniformLocation(id, name);
	}

	template<typename Type> void ShaderProgram::SetUniform(const char * uniformName, const Type & data) const {
		std::cout << "Invalid Data Type" << std::endl;
	}

	template<typename Type>
	void ShaderProgram::GetUniformValue(const char * uniformName, Type & data) const {
		std::cout << "Unknown Data Type" << std::endl;
	}

	template<> void ShaderProgram::GetUniformValue(const char * uniformName, bool & data) const {
		const GLint loc = GetUniformLocation(uniformName);
		int v = 0;
		glGetUniformiv(id, loc, &v);
		data = v > 0;
	}

	template<> void ShaderProgram::SetUniform<glm::mat3>(const char * uniformName, const glm::mat3 & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(data));
	}

	template<> void ShaderProgram::SetUniform<glm::mat4>(const char * uniformName, const glm::mat4 & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data));
	}

	template<> void ShaderProgram::SetUniform<glm::vec2>(const char * uniformName, const glm::vec2 & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniform2fv(loc, 1, glm::value_ptr(data));
	}

	template<> void ShaderProgram::SetUniform<glm::vec3>(const char * uniformName, const glm::vec3 & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniform3fv(loc, 1, glm::value_ptr(data));
	}

	template<> void ShaderProgram::SetUniform<glm::vec4>(const char * uniformName, const glm::vec4 & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniform4fv(loc, 1, glm::value_ptr(data));
	}

	template<> void ShaderProgram::SetUniform<int>(const char * uniformName, const int & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniform1i(loc, data);
	}

	template<> void ShaderProgram::SetUniform<float>(const char * uniformName, const float & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniform1f(loc, data);
	}

	template<> void ShaderProgram::SetUniform<bool>(const char * uniformName, const bool & data) const {
		Bind();
		const GLint loc = glGetUniformLocation(id, uniformName);
		glUniform1i(loc, data);
	}



	const GLuint ShaderProgram::GetID() const {
		return id;
	}

	std::vector<ShaderValue> ShaderProgram::GetAttributes() {
		GLint count;
		glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count);
		std::vector<ShaderValue> attributes;
		GLenum type;
		const GLsizei maxNameLength = 50;
		GLchar name[maxNameLength];
		GLsizei realNameLength;
		GLint size;
		for(GLint i = 0; i < count; i++) {
			glGetActiveAttrib(id, i, maxNameLength, &realNameLength, &size, &type, name);
			ShaderValue v;
			v.name.append(name, realNameLength);
			v.type = type;
			attributes.push_back(v);
		}
		return attributes;
	}

	std::vector<ShaderValue> ShaderProgram::GetUniforms() {
		GLint count;
		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
		std::vector<ShaderValue> uniforms;
		GLenum type;
		const GLsizei maxNameLength = 50;
		GLchar name[maxNameLength];
		GLsizei realNameLength;
		GLint size;
		for(GLint i = 0; i < count; i++) {
			glGetActiveUniform(id, i, maxNameLength, &realNameLength, &size, &type, name);
			ShaderValue v;
			v.name.append(name, realNameLength);
			v.type = type;
			uniforms.push_back(v);
		}
		return uniforms;
	}
	const bool ShaderProgram::operator==(const std::shared_ptr<ShaderProgram>& other) const {
		return id == other->id;
	}
}
