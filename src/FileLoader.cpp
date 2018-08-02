#include "FileLoader.h"
#include "TextParser.h"
#include "Mesh.h"
#include "Shader.h"
#include "TypeConverter.h"
#include "OpenGLUtility.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace B00289996B00227422 {
	// print shader errors
	void PrintShaderError(const GLuint & id) {
		GLint logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		GLchar * errorLog = new GLchar[logLength];
		int l = 0;
		glGetShaderInfoLog(id, logLength, &l, errorLog);
		std::cout << errorLog << std::endl;
	}
	// print shader program errors
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
		// delete any textures, all other objects take care of their own memory
		for(std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator i = textures.begin(); i != textures.end(); ++i) {
			glDeleteTextures(1, &(*i).second->id);
		}
	}

	std::vector<std::shared_ptr<Mesh>> FileLoader::LoadMeshes(const std::string filePath, const bool & singleInstance) {
		// if an instanced mesh is requested and an instance already exists, return that instance
		if(singleInstance && meshes.count(filePath) != 0) return meshes[filePath];
		else {
			Assimp::Importer importer; // create an importer
			// attempt to load the model
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
					// convert form assimps structure to the mesh class
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
					// obtain the indices of the relevant mesh
					for(unsigned int j = 0; j < mesh->mNumFaces; ++j) {
						const aiFace& face = mesh->mFaces[j];
						for(unsigned int k = 0; k < face.mNumIndices; k++) {
							indices.push_back(face.mIndices[k]);
						}
					}
					newMeshes.push_back(std::make_shared<Mesh>(std::string(mesh->mName.C_Str()), vertices, indices));
				}
				// if a instanced mesh was requested, save that instance for later use
				if(singleInstance) meshes[filePath] = newMeshes;
				return newMeshes;
			}
		}
		return std::vector<std::shared_ptr<Mesh>>();
	}

	std::shared_ptr<Texture> FileLoader::LoadTexture(const std::string filePath, const TextureType & type) {
		// if the texture already exists, return it
		if(textures.count(filePath) != 0) return textures[filePath];
		else {
			
			GLuint id;
			// activate the relevant texture type
			glActiveTexture(GL_TEXTURE0 + (unsigned int)type);
			// create a new texture
			glGenTextures(1, &id);
			if (id != GL_FALSE) {
				glBindTexture(GL_TEXTURE_2D, id);
				int width, height, channels;
				// have soil load the image data
				unsigned char * imageData = stbi_load(filePath.c_str(), &width, &height, &channels, STBI_rgb_alpha);
				if (imageData == nullptr) { // if the image data was not loaded
					// print an error and return an empty shared pointer
					std::cout << "Error loading image from " << filePath << std::endl;
					return std::shared_ptr<Texture>();
				}
				// populate the generated texure with the image data
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				// delete the image data
				stbi_image_free(imageData);
				// generate mipmaps
				glGenerateMipmap(GL_TEXTURE_2D);
				// set up texture parameter
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
				// create a texture stuct and populate it with the image data
				std::shared_ptr<Texture> t = std::make_shared<Texture>();
				t->id = id;
				t->type = type;
				t->width = width;
				t->height = height;
				textures[filePath] = t; // store the texture for re-use
				return t; // return the texture
			}
			std::cout << "Error loading image from " << filePath << std::endl;
			return std::shared_ptr<Texture>();
		}
		return std::shared_ptr<Texture>();
	}

	std::shared_ptr<ShaderProgram> FileLoader::LoadShader(const std::string & vert, const std::string & frag, const std::string & geom) {
		const std::string key = vert + frag + geom;
		if(shaders.count(key) != 0) return shaders[key]; // if the shader program has already been loaded, return it
		else {
			// try and load the shaders as strings
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
					// load the shaders
					return LoadShaderFromString(vertData, fragData, geomData);
				}
			}
			// load the shaders
			return LoadShaderFromString(vertData, fragData);
		}
		return std::shared_ptr<ShaderProgram>();
	}
	std::shared_ptr<ShaderProgram> FileLoader::LoadShaderFromString(const std::string & vert, const std::string & frag, const std::string & geom) {
		// create the required shaders
		GLuint v = glCreateShader(GL_VERTEX_SHADER);
		GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
		// compile the shader sources
		const char * vertSource = vert.c_str();
		GLint vSize = (GLint)vert.size();
		glShaderSource(v, 1, &vertSource, &vSize);
		glCompileShader(v);
		// check the status of the vertex shader
		GLint status;
		glGetShaderiv(v, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			PrintShaderError(v);
			return std::shared_ptr<ShaderProgram>();
		}

		const char * fragSource = frag.c_str();
		GLint fSize = (GLint)frag.size();
		glShaderSource(f, 1, &fragSource, &fSize);
		// check the status of the fragment shader
		glCompileShader(f);
		glGetShaderiv(f, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			PrintShaderError(f);
			return std::shared_ptr<ShaderProgram>();
		}
		// create the shader program struct
		std::shared_ptr<ShaderProgram> s = std::make_shared<ShaderProgram>();
		// generate the shader program and store its handle in the struct 
		s->id = glCreateProgram();
		// attach the vertex and fragment shaders
		glAttachShader(s->id, v);
		glAttachShader(s->id, f);
		s->shaderIDs.push_back(v);
		s->shaderIDs.push_back(f);

		if(geom.size() > 0) { // if geometry shader is included
			// compile it and link it to the shader program
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
		// bind the default attribute locations
		glBindAttribLocation(s->id, VERTEX_POSITION, "position");
		glBindAttribLocation(s->id, VERTEX_NORMAL, "normal");
		glBindAttribLocation(s->id, VERTEX_TANGENT, "tangent");
		glBindAttribLocation(s->id, VERTEX_BITANGENT, "bitangent");
		glBindAttribLocation(s->id, VERTEX_TEXCOORD, "texCoord");
		glBindAttribLocation(s->id, VERTEX_COLOUR, "colour");
		// link and bind the shader program
		s->Link();
		s->Bind();
		// check to ensure the shader progran id linked correctly
		glGetProgramiv(s->id, GL_LINK_STATUS, &status);
		if(status == GL_FALSE) {
			PrintShaderProgramError(s->id);
			return std::shared_ptr<ShaderProgram>();
		}
		shaders[vert + frag + geom] = s;
		return s;
	}
}