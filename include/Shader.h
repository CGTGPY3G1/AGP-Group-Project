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
		/// <summary>Initializes a new instance of the <see cref="ShaderProgram"/> class.</summary>
		ShaderProgram();
		/// <summary>Finalizes an instance of the <see cref="ShaderProgram"/> class.</summary>
		~ShaderProgram();
		/// <summary>Gets an attribute location (if that attribute exists).</summary>
		/// <param name="name">The name of the attribute.</param>
		/// <returns>the attribute location (if it exists), else some value below 0</returns>
		const GLint GetAttributeLocation(const GLchar *name) const;
		/// <summary>Binds an attribute location.</summary>
		/// <param name="index">The location to bind to.</param>
		/// <param name="name">The name of the attribute.</param>
		void BindAttributeLocation(GLuint index, const GLchar *name) const;
		/// <summary>Links the shader.</summary>
		void Link();
		/// <summary>Binds (uses) this shader.</summary>
		void Bind() const;
		/// <summary>Unbinds (stops using) the shader.</summary>
		void UnBind() const;
		/// <summary>Gets a uniform location.</summary>
		/// <param name="name">The name of the uniform.</param>
		/// <returns>the uniform location, if it exists, else some numebr below 0</returns>
		const GLuint GetUniformLocation(const char* name) const;
		/// <summary>Sets a uniforms data.</summary>
		/// <param name="uniformName">Name of the uniform.</param>
		/// <param name="data">The data.</param>
		template <typename Type> void SetUniform(const char* uniformName, const Type & data) const;
		/// <summary>Gets the shader id.</summary>
		/// <returns>the shader id</returns>
		const GLuint GetID() const;
		/// <summary>Gets a uniform value.</summary>
		/// <param name="uniformName">Name of the uniform.</param>
		/// <param name="data">a varable to place the data in data.</param>
		template <typename Type> void GetUniformValue(const char* uniformName, Type & data) const;
		/// <summary>Gets a list of this shaders attributes.</summary>
		/// <returns>a vector of shader attributes</returns>
		std::vector<ShaderValue> GetAttributes();
		/// <summary>Gets a list of this shaders uniforms.</summary>
		/// <returns>a vector of shader uniforms</returns>
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
