#include "FileLoader.h"
#include "TextParser.h"
#include "Mesh.h"
#include "Shader.h"
#include "TypeConverter.h"
#include "OpenGLUtility.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#include <soil.h>
#include <iostream>

namespace B00289996B00227422 {

	void PrintShaderError(const GLuint & id) {
		GLint logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		GLchar * errorLog = new GLchar[logLength];
		int l = 0;
		glGetShaderInfoLog(id, logLength, &l, errorLog);
		std::cout << errorLog << std::endl;
	}

	void PrintShaderProgramError(const GLuint & id) {
		GLint logLength = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		GLchar * errorLog = new GLchar[logLength];
		int l = 0;
		glGetProgramInfoLog(id, logLength, &l, errorLog);
		std::cout << errorLog << std::endl;
	}

	FileLoader & FileLoader::GetInstance() {
		static FileLoader instance;
		return instance;
	}

	FileLoader::~FileLoader() {
		for(std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator i = textures.begin(); i != textures.end(); ++i) {
			glDeleteTextures(1, &(*i).second->id);
		}
	}

	std::vector<std::shared_ptr<Mesh>> FileLoader::LoadMeshes(const std::string filePath, const bool & singleInstance) {
		if(singleInstance && meshes.count(filePath) != 0) return meshes[filePath];
		else {
			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes);
			if(!scene) {
				std::cout << "Unable to load mesh: " << importer.GetErrorString() << std::endl;
			}
			else {
				std::vector<std::shared_ptr<Mesh>> newMeshes;
				for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
					aiMesh* mesh = scene->mMeshes[i];
					std::vector<Vertex> vertices;
					std::vector<unsigned int> indices;
					for(unsigned int j = 0; j < mesh->mNumVertices; ++j) {
						aiVector3D pos = mesh->mVertices[j];
						aiVector3D uv = mesh->mTextureCoords[0][j];
						aiVector3D normal = mesh->mNormals[j];
						aiVector3D tangent = mesh->mTangents[j];
						aiVector3D biTangent = mesh->mBitangents[j];
						Vertex v = Vertex();
						
						v.position.x = pos.x; v.position.y = pos.y; v.position.z = pos.z;
						v.textureCoords.x = uv.x; v.textureCoords.y = uv.y;
						v.normal.x = normal.x; v.normal.y = normal.y; v.normal.z = normal.z;
						v.tangent.x = tangent.x; v.tangent.y = tangent.y; v.tangent.z = tangent.z;
						v.biTangent.x = biTangent.x; v.biTangent.y = biTangent.y; v.biTangent.z = biTangent.z;
						vertices.push_back(v);

					}
					for(unsigned int j = 0; j < mesh->mNumFaces; ++j) {
						const aiFace& face = mesh->mFaces[j];
						for(unsigned int k = 0; k < face.mNumIndices; k++) {
							indices.push_back(face.mIndices[k]);
						}
					}
					newMeshes.push_back(std::make_shared<Mesh>(std::string(mesh->mName.C_Str()), vertices, indices));
				}
				if(singleInstance) meshes[filePath] = newMeshes;
				return newMeshes;
			}
		}
		return std::vector<std::shared_ptr<Mesh>>();
	}

	std::shared_ptr<Texture> FileLoader::LoadTexture(const std::string filePath, const TextureType & type) {
		if(textures.count(filePath) != 0) return textures[filePath];
		else {
			GLuint id;
			glActiveTexture(GL_TEXTURE0 + (unsigned int)type);
			glGenTextures(1, &id);
			if (id != GL_FALSE) {
				glBindTexture(GL_TEXTURE_2D, id);
				int width, height;
				unsigned char * imageData = SOIL_load_image(filePath.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
				if (imageData == nullptr) {
					std::cout << "Error loading image from " << filePath << std::endl;
					return std::shared_ptr<Texture>();
				}
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				SOIL_free_image_data(imageData);

				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
				std::shared_ptr<Texture> t = std::make_shared<Texture>();
				t->id = id;
				t->type = type;
				t->width = width;
				t->height = height;
				textures[filePath] = t;
				return t;
			}
			std::cout << "Error loading image from " << filePath << std::endl;
			return std::shared_ptr<Texture>();
		}
		return std::shared_ptr<Texture>();
	}

	std::shared_ptr<ShaderProgram> FileLoader::LoadShader(const std::string & vert, const std::string & frag, const std::string & geom) {
		const std::string key = vert + frag + geom;
		if(shaders.count(key) != 0) return shaders[key];
		else {
			const std::string vertData = TextParser::ReadFile(vert);
			if(vertData.empty()) {
				std::cout << "No vertex shader data found" << std::endl;
				return std::shared_ptr<ShaderProgram>();
			}
			const std::string fragData = TextParser::ReadFile(frag);
			if(fragData.empty()) {
				std::cout << "No fragment shader data found" << std::endl;
				return std::shared_ptr<ShaderProgram>();
			}
			if(geom.size() > 0) {
				const std::string geomData = TextParser::ReadFile(geom);
				if(geomData.empty()) {
					std::cout << "No Geometry shader data found" << std::endl;
				}
				else {
					return LoadShaderFromString(vertData, fragData, geomData);
				}
			}
			return LoadShaderFromString(vertData, fragData);
		}
		return std::shared_ptr<ShaderProgram>();
	}
	std::shared_ptr<ShaderProgram> FileLoader::LoadShaderFromString(const std::string & vert, const std::string & frag, const std::string & geom) {
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		const char * vertSource = vert.c_str();
		GLint vSize = (GLint)vert.size();
		glShaderSource(v, 1, &vertSource, &vSize);
		glCompileShader(v);

		GLint status;
		glGetShaderiv(v, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			PrintShaderError(v);
			return std::shared_ptr<ShaderProgram>();
		}

		const char * fragSource = frag.c_str();
		GLint fSize = (GLint)frag.size();
		glShaderSource(f, 1, &fragSource, &fSize);
		glCompileShader(f);
		glGetShaderiv(f, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			PrintShaderError(f);
			return std::shared_ptr<ShaderProgram>();
		}

		std::shared_ptr<ShaderProgram> s = std::make_shared<ShaderProgram>();
		s->id = glCreateProgram();
		glAttachShader(s->id, v);
		glAttachShader(s->id, f);
		s->shaderIDs.push_back(v);
		s->shaderIDs.push_back(f);

		if(geom.size() > 0) {
			GLuint g = glCreateShader(GL_GEOMETRY_SHADER);
			const char * geomSource = geom.c_str();
			GLint gSize = (GLint)geom.size();
			glShaderSource(g, 1, &geomSource, &gSize);
			glCompileShader(g);
			glGetShaderiv(g, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE) {
				PrintShaderError(g);
			}
			else {
				glAttachShader(s->id, g);
				s->shaderIDs.push_back(g);
			}
		}

		glBindAttribLocation(s->id, VERTEX_POSITION, "position");
		glBindAttribLocation(s->id, VERTEX_NORMAL, "normal");
		glBindAttribLocation(s->id, VERTEX_TANGENT, "tangent");
		glBindAttribLocation(s->id, VERTEX_BITANGENT, "bitangent");
		glBindAttribLocation(s->id, VERTEX_TEXCOORD, "texCoord");
		glBindAttribLocation(s->id, VERTEX_COLOUR, "colour");
		s->Link();
		s->Bind();
		glGetProgramiv(s->id, GL_LINK_STATUS, &status);
		if(status == GL_FALSE) {
			PrintShaderProgramError(s->id);
			return std::shared_ptr<ShaderProgram>();
		}
		shaders[vert + frag + geom] = s;
		return s;
	}
}