#include "texture.hpp"
#include "compiler_settings.h"

Texture::Texture(std::string filename) : _filename(filename) {
	std::cout << "_ Texture cons by filename: " << filename.c_str() << std::endl;

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

	this->_isLoaded = false;
	this->_width = bmpInfo->biWidth;
	this->_height = bmpInfo->biHeight;

	// Checks
	if (bmpHeader->bfType != 0x4D42 ) {
		std::cout << "File \"" << filename.c_str() << "\" isn't a bitmap file\n";
		exit(2);
	} else if (bmpInfo->biWidth < 0 || bmpInfo->biHeight < 0) {
		std::cout << "File \"" << filename.c_str() << "\" has a negative width or height. Not supported (yet).\n";
		exit(2);
	}
	GLenum bitMode;
	int pixelSize = bmpInfo->biBitCount / 8;
	if (bmpInfo->biBitCount == 24) {
		bitMode = GL_RGB;
	} else if (bmpInfo->biBitCount == 32) {
		bitMode = GL_RGBA;
	} else {
		std::cout << "File \"" << filename.c_str() << "\" isn't a 24/32 bits bitmap file\n";
		exit(2);
	}

	int row_size_used = this->_width * pixelSize;
	int padding = (4 - (row_size_used % 4)) % 4;
	int row_size_full = row_size_used + padding;
	std::cout << "row_size_used:" << row_size_used << std::endl;
	std::cout << "padding:" << padding << std::endl;
	std::cout << "row_size_full:" << row_size_full << std::endl;
	std::cout << "bmpInfo->biSize:" << bmpInfo->biSize << std::endl;
	std::cout << "bmpInfo->biWidth:" << bmpInfo->biWidth << std::endl;
	std::cout << "bmpInfo->biHeight:" << bmpInfo->biHeight << std::endl;
	std::cout << "bmpInfo->biCompression:" << bmpInfo->biCompression << std::endl;
	std::cout << "bmpInfo->biSizeImage:" << bmpInfo->biSizeImage << std::endl;
	// if (!bmpInfo->biSizeImage)//Specifies the size, in bytes, of the image. This can be set to 0 for uncompressed RGB bitmaps.
		bmpInfo->biSizeImage = row_size_full * bmpInfo->biHeight;
	std::cout << "bmpInfo->biSizeImage:" << bmpInfo->biSizeImage << " (computed)"<< std::endl;
	std::cout << "bmpInfo->biBitCount:" << bmpInfo->biBitCount << std::endl;

	unsigned int RGBsize = (this->_width * 3) * bmpInfo->biHeight;// 3 cauz RGB
	std::cout << "RGBsize:" << RGBsize << std::endl;
	uint8_t*	pixels = new uint8_t[bmpInfo->biSizeImage];
	// this->_data = new uint8_t[bmpInfo->biSizeImage];//oversized, but well padded for opengl (contains padding and potential 32bit size)
	this->_data = new uint8_t[RGBsize];//size can be not well padded for openGL, see Texture::loadTexture();

	// Go to where image data starts, then read in image data
	file.seekg(bmpHeader->bfOffBits);
	file.read((char*)pixels, bmpInfo->biSizeImage);

	std::cout << "transfering...";
	/*
		copy BGR data, omitting padding
		ex:
		BGRBGRBGR...000
		BGRBGRBGR...000
		BGRBGRBGR...000
		BGRBGRBGR...000
	*/
	int j = 0;
	for (int i = 0; i < bmpInfo->biSizeImage; i++) {
		if (bitMode == GL_RGBA && i % 4 == 3) {
			// std::cout << "fuck A:" << (int)pixels[i] << std::endl;
		} else if (padding && (i % row_size_full) > row_size_used - 1) {
			// std::cout << "fuck padding" << std::endl;
		} else {
			this->_data[j] = pixels[i];
			j++;
		}
	}
	std::cout << " Done" << std::endl;

	/*
		.bmp files store image data in the BGR format, and we have to convert it to RGB.
		Just swap Red and Blue values.
		This can be avoided by using GL_BGR instead of GL_RGB with glTexImage2D() func
		note: the data is stocked as lines, from last line to 1st line
	*/
	std::cout << "converting BGR to RGB...";
	uint8_t tmp = 0;
	for (unsigned long i = 0; i < RGBsize; i += 3) {
		tmp = this->_data[i + 0];
		this->_data[i + 0] = this->_data[i + 2];
		this->_data[i + 2] = tmp;
	}
	std::cout << " Done" << std::endl;
	//construct GL Texture
	this->loadTexture();

	delete bmpHeader;
	delete bmpInfo;
	delete[] pixels;
	std::cout << __PRETTY_FUNCTION__ << " END" << std::endl;
	std::cout << "----------------------------------------\n" << std::endl;
}

Texture::Texture(uint8_t* data, unsigned int width, unsigned int height) : _width(width), _height(height) {
	this->_filename = "N/A (data)";
	int size = width * height * 3;
	this->_data = new uint8_t[size];
	memcpy(this->_data, data, size);
	this->loadTexture();
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
	this->_isLoaded = src.isLoaded();
	if (this->_isLoaded)
		this->loadTexture();
	return (*this);
}

Texture::~Texture() {
	// cout << "_ Texture des" << endl;
	delete[] this->_data;
	this->unloadTexture();
}

void	Texture::updateData(uint8_t* data, unsigned int width, unsigned int height) {
	//force the user to send widht and height, adds a layer of security in case of unadapted data
	if (this->_width != width || this->_height != height) {
		std::cout << "Texture::updateData(...) failed: wrong width and/or height\n";
		std::cout << this->_width << "x" << this->_height << " != " << width << "x" << height << "\n";
		std::cout << this->_id << ": " << this->_filename << std::endl;
		//exit(4);
	} else {
		memcpy(this->_data, data, width * height);
		if (this->_isLoaded) {
			glBindTexture(GL_TEXTURE_2D, this->_id);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
}


void	Texture::printData() const {
	unsigned int size = this->_width * this->_height * 3;
	for (unsigned int i = 0; i < size; i++) {
		if (i % (3 * this->_width) == 0)
			std::cout << "\tline " << (i + 3) / (this->_width * 3) << std::endl;
		std::cout << (unsigned int)(this->_data[i]) << ":";
		if (i % 3 == 2)
			std::cout << std::endl;
	}
}

void			Texture::loadTexture() {
	this->_isLoaded = true;
	// cout << this->_id << endl;
	glGenTextures(1, &this->_id);
	glBindTexture(GL_TEXTURE_2D, this->_id);

	//https://stackoverflow.com/questions/7380773/glteximage2d-segfault-related-to-width-height
	//to avoid doing that, use bmp with width and height being even numbers
	// can slow down performence ?
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_width, this->_height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// GL_NEAREST GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_REPEAT GL_MIRRORED_REPEAT GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Texture::unloadTexture() {
	this->_isLoaded = false;
	this->_id = -1;
	glDeleteTextures(1, &this->_id);
}


//accessors
GLuint			Texture::getId() const { return (this->_id); }
std::string		Texture::getFilename() const { return (this->_filename); }
unsigned int	Texture::getWidth() const { return (this->_width); }
unsigned int	Texture::getHeight() const { return (this->_height); }
uint8_t*		Texture::getData() const { return (this->_data); }
bool			Texture::isLoaded() const { return (this->_isLoaded); }
