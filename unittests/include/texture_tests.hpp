#pragma once

#include "unittests.hpp"
#include "texture.hpp"

#define TEXTURE_DIR		"../../SimpleGL/images/"

class UnitTests::TextureTests {
public:
	void	setUp();
	void	tearDown();
	void	testEverything() const;

	void	testTexture() const;
	void	testTexture_operatorEqual() const;
	void	testTexture_getId() const;
	void	testTexture_getFilename() const;
	void	testTexture_getWidth() const;
	void	testTexture_getHeight() const;
	void	testTexture_getData() const;
	void	testTexture_genTexture() const;
	// void	testTexture_() const;

private:
	Texture			*textureLena;
	std::string		lena;
	std::string		no_read;
	std::string		corrupted;
	std::string		invalid;
	std::string		in_dir_no_read;
	unsigned int	widthA_512;
	unsigned int	heightA_512;
};