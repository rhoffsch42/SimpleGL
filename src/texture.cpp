#include "simplegl.h"
#include "texture.hpp"

Texture::Texture(std::string filename) : _filename(filename) {
	cout << "_ Texture cons by filename: " << filename.c_str() << endl;

	filename = Misc::crossPlatPath(filename);
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cout << "Failure to open bitmap file.\n";
		exit(1);
	}

	// Allocate byte memory that will hold the two headers and read headers
	BMPFILEHEADER* bmpHeader = new BMPFILEHEADER;
	BMPINFOHEADER* bmpInfo = new BMPINFOHEADER;
	file.read((char*)bmpHeader, sizeof(BMPFILEHEADER));
	file.read((char*)bmpInfo, sizeof(BMPINFOHEADER));

	this->_width = bmpInfo->biWidth;
	this->_height = bmpInfo->biHeight;

	// Check if the file is an actual BMP file
	if (bmpHeader->bfType != 0x4D42) {
		std::cout << "File \"" << filename.c_str() << "\" isn't a bitmap file\n";
		exit(2);
	}

	// Go to where image data starts, then read in image data
	uint8_t*	pixels = new uint8_t[bmpInfo->biSizeImage];
	file.seekg(bmpHeader->bfOffBits);
	file.read((char*)pixels, bmpInfo->biSizeImage);

	/*
		copy BGR data, omitting padding
		ex:
		BGRBGRBGR...000
		BGRBGRBGR...000
		BGRBGRBGR...000
		BGRBGRBGR...000
	*/
	unsigned int size = this->_width * this->_height * 3;
	this->_data = new uint8_t[size];
	int row_size_used = this->_width * 3;
	int padding = (4 - (row_size_used % 4)) % 4;
	int row_size_full = row_size_used + padding;

	int j = 0;
	for (int i = 0; i < bmpInfo->biSizeImage; i++) {
		this->_data[j] = pixels[i];
		j++;
		if ((i % row_size_full) == (row_size_used - 1))
			i += padding;
	}

	/*
		.bmp files store image data in the BGR format, and we have to convert it to RGB.
		Just swap Red and Blue values.
		This can be avoided by using GL_BGR instead of GL_RGB with glTexImage2D() func
		note: the data is stocked as lines, from last line to 1st line
	*/
	uint8_t tmp = 0;
	for (unsigned long i = 0; i < size; i += 3) {
		tmp = this->_data[i + 0];
		this->_data[i + 0] = this->_data[i + 2];
		this->_data[i + 2] = tmp;
	}

	//construct GL Texture
	this->genTexture();

	delete bmpHeader;
	delete bmpInfo;
	delete[] pixels;
}

Texture::Texture(uint8_t* data, unsigned int width, unsigned int height) : _width(width), _height(height) {
	this->_filename = "N/A (data)";
	int size = width * height * 3;
	this->_data = new uint8_t[size];
	for (int i = 0; i < size; i++)
		this->_data[i] = data[i];
	this->genTexture();
}

Texture::Texture(const Texture& src) {
	// cout << "_ Texture cons by copy" << endl;
	*this = src;
}

Texture&	Texture::operator=(const Texture& src) {
	this->_filename = src.getFilename();
	this->_width = src.getWidth();
	this->_height = src.getHeight();
	unsigned int	size = this->_width * this->_height * 3;
	this->_data = new uint8_t[size];
	memcpy(this->_data, src.getData(), size);
	this->genTexture();
	return (*this);
}

Texture::~Texture() {
	// cout << "_ Texture des" << endl;
	delete[] this->_data;
}

void	Texture::printData() const {
	unsigned int size = this->_width * this->_height * 3;
	for (unsigned int i = 0; i < size; i++) {
		if (i % (3 * this->_width) == 0)
			cout << "\tline " << (i + 3) / (this->_width * 3) << endl;
		cout << (unsigned int)(this->_data[i]) << ":";
		if (i % 3 == 2)
			cout << endl;
	}
}

void			Texture::genTexture() {
	glGenTextures(1, &this->_id);
	// cout << this->_id << endl;
	glBindTexture(GL_TEXTURE_2D, this->_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_width, this->_height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//accessors
GLuint			Texture::getId() const { return (this->_id); }
std::string		Texture::getFilename() const { return (this->_filename); }
unsigned int	Texture::getWidth() const { return (this->_width); }
unsigned int	Texture::getHeight() const { return (this->_height); }
uint8_t*		Texture::getData() const { return (this->_data); }