#pragma once

#ifndef B00289996B00227422_HUD_H
#define B00289996B00227422_HUD_H
#include <SDL_ttf.h>
#include <GL/glew.h>
#include <vector>
#include <memory>
#include <glm\mat4x4.hpp>
namespace B00289996B00227422 {
	/// <summary>
	/// Horizontal allignment
	/// </summary>
	enum Alignment {
		Centered,
		Left,
		Right
	};
	class Mesh;
	struct Texture;
	class ShaderProgram;

	/// <summary>
	/// texture id and transform of item to be drawn
	/// </summary>
	struct HUDItem {
		std::shared_ptr<Texture> texture = nullptr;
		glm::mat4 transform = glm::mat4(1.0f);
	};

	struct FBOData : HUDItem {
		std::shared_ptr<ShaderProgram> shader;
	};
	
	/// <summary> the Heads Up Display </summary>
	class HUD {
	public:
		/// <summary> Gets the Singleton instance. </summary>
		/// <returns>the Singleton instance</returns>
		static HUD & GetInstance();
		/// <summary> Finalizes an instance of the <see cref="HUD"/> class. </summary>
		~HUD();
		/// <summary> Loads the font. </summary>
		/// <param name="fontName">Name of the font.</param>
		void LoadFont(const char * fontName);
		/// <summary> Draws text to the screen. </summary>
		/// <param name="toDraw">The text to draw.</param>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="alignment">The alignment.</param>
		void DrawText(const std::string & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment = Alignment::Centered);
		/// <summary> Draws text to the screen. width will be calculated automatically from the height and number of characters </summary>
		/// <param name="toDraw">The text to draw.</param>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		/// <param name="height">The height.</param>
		/// <param name="alignment">The alignment.</param>
		void DrawText(const std::string & toDraw, const float & x, const float & y, const float & height, const Alignment & alignment = Alignment::Centered);
		/// <summary> Draws s texture to the screen. </summary>
		/// <param name="toDraw">The texture to draw.</param>
		/// <param name="x">The x position.</param>
		/// <param name="y">The y position.</param>
		/// <param name="width">The width.</param>
		/// <param name="height">The height.</param>
		/// <param name="alignment">The alignment.</param>
		void DrawTexture(const std::shared_ptr<Texture> & toDraw, const float & x, const float & y, const float & width, const float & height, const Alignment & alignment = Alignment::Centered);


		/// <summary> Draws the provided FBO using either a (optional) provided shader, or the default hud shader. </summary>
		/// <param name="toDraw">To FBO to be drawn.</param>
		/// <param name="shader">(optional) The shader to draw with.</param>
		void DrawFBO(const std::shared_ptr<Texture> & toDraw, std::shared_ptr<ShaderProgram> shader = std::shared_ptr<ShaderProgram>());
		/// <summary> Renders the HUD. </summary>
		void Render();
		/// <summary> Sets the default shader.</summary>
		/// <param name="shader">The shader.</param>
		void SetShader(std::shared_ptr<ShaderProgram> shader);
	private:
		/// <summary> Prevents a default instance of the <see cref="HUD"/> class from being created. </summary>
		HUD();
		/// <summary> Converts text to a texture. </summary>
		/// <param name="str">The text.</param>
		/// <param name="textID">The texture to draw to.</param>
		/// <returns></returns>
		std::shared_ptr<Texture>  textToTexture(const char * str, const std::shared_ptr<Texture> &  textID);
		TTF_Font* font; // the font
		std::vector<HUDItem> textObjects, textureObjects;
		std::vector<FBOData> fboData;
		unsigned int currentTextIndex, currentTextureIndex; // used to track how many items are to be drawn
		std::shared_ptr<Mesh> model;
		std::shared_ptr<ShaderProgram> shader;
	};
}
#endif // !B00289996B00227422_HUD_H



