#include "texture_tests.hpp"

void	UnitTests::TextureTests::setUp() {
	lena = TEXTURE_DIR;				lena.append("lena.bmp");
	no_read = TEXTURE_DIR;			no_read.append("no_read.bmp");
	corrupted = TEXTURE_DIR;		corrupted.append("corrupted.bmp");
	invalid = TEXTURE_DIR;			invalid.append("invalid.bmp");
	in_dir_no_read = TEXTURE_DIR;	in_dir_no_read.append("in_dir_no_read.bmp");

	widthA_512 = 512;
	heightA_512 = 512;

	if (!glfwInit()) {
		std::cerr << "glfwInit failed" << std::endl;
		Misc::breakExit(GL_ERROR);
	}

	textureLena = new Texture(lena);
}

void	UnitTests::TextureTests::tearDown() {
	delete textureLena;
}
void	UnitTests::TextureTests::testEverything() const {
	// std::cout << "texture tests" << std::endl;
	testTexture();
	testTexture_operatorEqual();
	testTexture_getId();
	testTexture_getFilename();
	testTexture_getWidth();
	testTexture_getHeight();
	testTexture_getData();
	testTexture_genTexture();
}

void	UnitTests::TextureTests::testTexture() const {
	Texture		texture1(lena);
	TEST("Texture()", texture1._filename == lena);
	TEST("Texture()", texture1._width == widthA_512);
	TEST("Texture()", texture1._height == heightA_512);
	TEST("Texture()", texture1._data != NULL);
}
void	UnitTests::TextureTests::testTexture_operatorEqual() const {
	Texture		texture2 = *textureLena;
	TEST("Texture operator=", texture2._filename == textureLena->_filename);
	TEST("Texture operator=", texture2._width == textureLena->_width);
	TEST("Texture operator=", texture2._height == textureLena->_height);
	TEST("Texture operator=", texture2._data != textureLena->_data);
	TEST("Texture operator=", memcmp(texture2._data, textureLena->_data, texture2._width * texture2._height) == 0);
	std::cout << UT_ERROR << "  this operator should never be used, see TextureTests::testTexture_operatorEqual() in texture_tests.cpp" << std::endl;
	/*
		its useless to copy the data, more: its useless to copy the entire Texture class
		if we want to copy a texture and rebuild it (modified or not), we have to use the following constructor:
			Texture(uint8_t* data, unsigned int width, unsigned int height);
		EXCEPT IF:
			opengl use specificaly THAT data and not a copy of that data in the graphic card memory (can we reload it?)
		We have to check that
	*/
}
void	UnitTests::TextureTests::testTexture_getId() const {
	GLuint	id = textureLena->getId();
	TEST("Texture getId", id == textureLena->_id);
}
void	UnitTests::TextureTests::testTexture_getFilename() const {
	std::string	file = textureLena->getFilename();
	TEST("Texture getId", file == textureLena->_filename);
}
void	UnitTests::TextureTests::testTexture_getWidth() const {
	unsigned int	width = textureLena->getWidth();
	TEST("Texture getWidth", width == textureLena->_width);
}
void	UnitTests::TextureTests::testTexture_getHeight() const {
	unsigned int	height = textureLena->getHeight();
	TEST("Texture getHeight", height == textureLena->_height);
}
void	UnitTests::TextureTests::testTexture_getData() const {
	uint8_t		*data = textureLena->getData();
	TEST("Texture getData", data == textureLena->_data);
}
void	UnitTests::TextureTests::testTexture_genTexture() const {
	NOT_TESTED("Texture genTexture", " : need a human touch (check the texture visually");
}