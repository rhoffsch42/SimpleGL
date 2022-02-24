#pragma once

#include "program.hpp"
#include "math.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include <map>

/*
	https://learnopengl.com/In-Practice/Text-Rendering
*/

/// Holds all state information relevant to a character as loaded using FreeType
class Character {
public:
	//Character() : TexturSize(ivec2()), Bearing(ivec2()) {}
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text : public Object
{
public:
	std::string		text;
	Math::Vector3	color;
};

class TextPG : public Program
{
public:
	static std::string	fonts_folder;

	TextPG(std::string vs_file, std::string fs_file, bool init_locations = true);
	int	init_freetype(std::string font, float width = 800, float height = 600);
	~TextPG();
	virtual void	render(Object& object, Math::Matrix4 VPmatrix) const;
	void	render(std::string text, float x, float y, float scale, Math::Vector3 color) const;
	virtual void	renderObjects(list<Object*>& list, Cam& cam, unsigned int flags = 0);

protected:
	//slots
	GLint		_projection; // matrix
	GLint		_textColor;

	virtual void	getLocations();
private:
	std::map<GLchar, Character>	characters;

	GLuint	_vao;
	GLuint	_vbo;
};