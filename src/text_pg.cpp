#include "simplegl.h"
#include "text_pg.hpp"
#include "misc.hpp"
#include <utility>
#include <iostream>
//#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> // glm::ortho
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#ifdef SGL_DEBUG
 #define SGL_TEXT_PG_DEBUG
#endif
#ifdef SGL_TEXT_PG_DEBUG 
 #define D(x) std::cout << "[TextPG] " << x ;
 #define D_(x) x
 #define D_SPACER "-- text_pg.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

std::string	TextPG::fonts_folder = Misc::getCurrentDirectory() + "fonts/";

TextPG::TextPG(std::string vs_file, std::string fs_file, bool init_locations)
	: Program(vs_file, fs_file)
{
	this->_projection = 0;
	this->_textColor = 0;
	this->_vao = 0;
	this->_vbo = 0;
	//D("_ " << __PRETTY_FUNCTION__ << std::endl)
	if (init_locations)
		this->getLocations();
	//D(__PRETTY_FUNCTION__ << " END" << std::endl)
	D("----------------------------------------\n" << std::endl)
}

TextPG::~TextPG() {
	//D("_ " << __PRETTY_FUNCTION__ << std::endl)
}

void	TextPG::render(Object& object, Math::Matrix4 VPmatrix) const {
	//void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
	Text* obj = dynamic_cast<Text*>(&object);
	if (!obj) {
		D("dynamic_cast<Text*> failed on Object : " << obj << std::endl)
		// Misc::breakExit(22);
		// this can happen when an object is being nulled to be replaced by another one, block or continue?
		return;
	}

	//std::string		text = obj->text;
	//float			scale = obj->local.getScale().x;
	//Math::Vector3	color = obj->color;
	Math::Vector3	pos = obj->local.getPos();
	this->render(obj->text, pos.x, pos.y, obj->local.getScale().x, obj->color);
}

void	TextPG::render(std::string text, float x, float y, float scale, Math::Vector3 color) const {
	// activate corresponding render state	
	glUseProgram(this->_program);
	glUniform3f(this->_textColor, color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->_vao);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		//Character ch = this->characters[*c];
		Character cho = this->characters.find(*c)->second;
		Character* ch = &cho;
		if (!ch) {
			D("ficl\n")
			Misc::breakExit(-44);
		}

		float xpos = x + ch->Bearing.x * scale;
		float ypos = y - (ch->Size.y - ch->Bearing.y) * scale;

		float w = ch->Size.x * scale;
		float h = ch->Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch->TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch->Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
 }

void	TextPG::renderObjects(std::list<Object*>& list, Cam& cam, unsigned int flags) {}

void	TextPG::getLocations() {
	/* refacto: envoyer la fonction dans la classe Program:
		getLocations(stringList Uniform, stringList Attribute)
		stock dans un map string:int (varname:slot)

		pros:
			comportement uniforme entre obj3d/skybox/autre
		cons:
			plus lent a l'utilisation ?
	*/

	/*
		true	glGetUniformLocation
		false	glGetAttribLocation
	*/

	//D("_ " << __PRETTY_FUNCTION__ << " : " << this->_program << std::endl)
	this->_projection = this->getSlot("projection", true);
	this->_textColor = this->getSlot("textColor", true);
}

int		TextPG::init_freetype(std::string font, float width, float height) {
	glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
	glUseProgram(this->_program);
	//shader.use();
	glUniformMatrix4fv(this->_projection, 1, GL_FALSE, glm::value_ptr(projection));

	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		D("ERROR::FREETYPE: Could not init FreeType Library" << std::endl)
		return -1;
	}

	// find path to font
	//FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
	std::string font_name = TextPG::fonts_folder + font;
	if (font_name.empty())
	{
		D("ERROR::FREETYPE: Failed to load font_name: " << font_name << std::endl)
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		D("ERROR::FREETYPE: Failed to load font" << std::endl)
		return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (int c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, (GLchar)c, FT_LOAD_RENDER))
			{
				D("ERROR::FREETYTPE: Failed to load Glyph" << std::endl)
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			characters.insert(std::pair<GLchar, Character>((GLchar)c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &this->_vao);
	glGenBuffers(1, &this->_vbo);
	glBindVertexArray(this->_vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}
