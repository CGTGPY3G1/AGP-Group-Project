#include "HUD.h"
#include "Camera.h"
#include "GameObject.h"
#include "FileLoader.h"
#include "Texture.h"
#include "Mesh.h"
#include "GameObjectManager.h"
#include "MeshRenderer.h"
#include "Graphics.h"
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace B00289996B00227422 {
	HUD::HUD() :currentTextIndex(0), currentTextureIndex(0) {
		// ensure TTF is only initialised once, (irrelevant due to use of the singleton pattern)
		static bool ttfInitialized = false;
		if(!ttfInitialized) {
			if(TTF_Init() == -1) std::cout << "TTF failed to initialise." << std::endl;
			else ttfInitialized = true;
		}
		LoadFont("arial.ttf");
		std::vector<std::shared_ptr<Mesh>> meshes = FileLoader::GetInstance().LoadMeshes("OBJModels//quad.obj", false);
		model = meshes[0];
	}

	HUD & HUD::GetInstance() {
		static HUD instance;
		return instance;
	}

	HUD::~HUD() {

	}

	void HUD::LoadFont(const char * fontName) {
		font = TTF_OpenFont(fontName, 24);
		if(font == NULL) std::cout << "Failed to open font." << std::endl;
	}

	void HUD::DrawText(const std::string & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment) {
		if(toDraw.size() > 0) {
			glm::mat4 transform(1.0f);
			// set the text position relative to the specified alignment
			transform = glm::translate(transform, alignment == Alignment::Centered ? glm::vec3(x, y, 0.0f) : alignment == Alignment::Left ? glm::vec3(x + width * 0.5f, y, 0.0f) : glm::vec3(x - width * 0.5f, y, 0.0f));
			// scale the text to the specified width and height
			transform = glm::scale(transform, glm::vec3(width, -height, 1.0f));

			if(currentTextIndex >= textObjects.size()) {// if there is no more room in the array
														// create the new text
				HUDItem text;
				text.texture = std::make_shared<Texture>();
				text.texture = textToTexture(toDraw.c_str(), text.texture);
				text.transform = transform;
				textObjects.push_back(text);
			}
			else {
				// update the old text
				textObjects[currentTextIndex].texture = textToTexture(toDraw.c_str(), textObjects[currentTextIndex].texture);
				textObjects[currentTextIndex].transform = transform;
			}
			currentTextIndex++;
		}
	}

	void HUD::DrawText(const std::string & toDraw, const float & x, const float & y, const float & height, const Alignment & alignment) {
		if (toDraw.size() > 0) {
			glm::mat4 transform(1.0f);
			float width = (height * 0.2f) * toDraw.size();
			// set the text position relative to the specified alignment
			transform = glm::translate(transform, alignment == Alignment::Centered ? glm::vec3(x, y, 0.0f) : alignment == Alignment::Left ? glm::vec3(x + width, y, 0.0f) : glm::vec3(x - width, y, 0.0f));
			// scale the text to the specified width and height
			transform = glm::scale(transform, glm::vec3(width, -height, 1.0f));

			if (currentTextIndex >= textObjects.size()) {// if there is no more room in the array
														 // create the new text
				HUDItem text;
				text.texture = std::make_shared<Texture>();
				text.texture = textToTexture(toDraw.c_str(), text.texture);
				text.transform = transform;
				textObjects.push_back(text);
			}
			else {
				// update the old text
				textObjects[currentTextIndex].texture = textToTexture(toDraw.c_str(), textObjects[currentTextIndex].texture);
				textObjects[currentTextIndex].transform = transform;
			}
			currentTextIndex++;
		}
	}

	void HUD::DrawTexture(const std::shared_ptr<Texture> & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment) {
		if(toDraw) {
			glm::mat4 transform(1.0f);
			// set the textures position relative to the specified alignment
			transform = glm::translate(transform, alignment == Alignment::Centered ? glm::vec3(x, y, 0.0f) : alignment == Alignment::Left ? glm::vec3(x + width * 0.5f, y, 0.0f) : glm::vec3(x - width * 0.5f, y, 0.0f));

			// scale the texture to the specified width and height
			transform = glm::scale(transform, glm::vec3(width, height, 1.0f));

			if(currentTextureIndex >= textureObjects.size()) { // if there is no more room in the array
															   // create the new texture
				HUDItem texture;
				texture.texture = toDraw;
				texture.transform = transform;
				textureObjects.push_back(texture);
			}
			else {
				// update the old texture
				textureObjects[currentTextureIndex].texture = toDraw;
				textureObjects[currentTextureIndex].transform = transform;
			}
			currentTextureIndex++;
		}
	}

	void HUD::DrawFBO(const std::shared_ptr<Texture>& toDraw, std::shared_ptr<ShaderProgram> shader) {
		// if a shader has been provided, use it, else, use the default shader
		std::shared_ptr<ShaderProgram> drawShader = shader ? shader : this->shader;
		if(drawShader) { 
			drawShader->Bind(); //bind the shader
			const static glm::mat4 transform(1.0f); // a static identity matrix (will only be initialized once)
			toDraw->Bind(drawShader); // bind the texture
			drawShader->SetUniform("model", transform);
			model->Render();
		}
	}

	void HUD::Render() {
		if(shader) {
			shader->Bind();
			//Disable depth test and culling for HUD label
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			
			for(unsigned int i = 0; i < currentTextureIndex; i++) {
				textureObjects[i].texture->Bind(shader);
				shader->SetUniform("model", textureObjects[i].transform);
				model->Render();
			}
			currentTextureIndex = 0;
			for (unsigned int i = 0; i < currentTextIndex; i++) {
				textObjects[i].texture->Bind(shader);
				shader->SetUniform("model", textObjects[i].transform);
				model->Render();
			}
			currentTextIndex = 0;
		}
		//re-enable depth test and culling
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		shader->UnBind();
	}

	void HUD::SetShader(std::shared_ptr<ShaderProgram> shader) {
		this->shader = shader;
		// set the viewprojection value of the shader to an identity matrix to prevent the need for reorientation and scaling
		if(this->shader) this->shader->SetUniform("viewprojection", glm::mat4(1.0f)); 
	}

	std::shared_ptr<Texture> HUD::textToTexture(const char * str, const std::shared_ptr<Texture> & texture) {
		TTF_Font * font = this->font;

		SDL_Surface * stringImage = TTF_RenderText_Blended(font, str, {255, 255, 255});

		if(stringImage == NULL) {
			std::cout << "String surface not created." << std::endl;
		}

		if(texture->id == 0) {
			glGenTextures(1, &texture->id);//This avoids memory leakage, only initialise //first time
		}

		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringImage->w, stringImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stringImage->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);

		SDL_FreeSurface(stringImage);
		return texture;
	}
}
