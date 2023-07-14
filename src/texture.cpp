#include "simplegl.h"
#include "texture.hpp"
#include "compiler_settings.h"

#ifdef SGL_DEBUG
 #define SGL_TEXTURE_DEBUG
#endif
#ifdef SGL_TEXTURE_DEBUG 
 #define D(x) std::cout << "[Texture] " << x
 #define D_(x) x
 #define D_SPACER "-- texture.cpp -------------------------------------------------\n"
 #define D_SPACER_END "----------------------------------------------------------------\n"
#else 
 #define D(x)
 #define D_(x)
 #define D_SPACER ""
 #define D_SPACER_END ""
#endif

Texture::Texture(std::string filename) : _filename(filename) {
	D(D_SPACER);
	D("Texture cons by filename: " << filename.c_str() << "\n");
	D_(std::cout << "Hi");

	filename = Misc::crossPlatPath(filename);
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		D(Misc::getCurrentDirectory() << "\n");
		D("Failure to open bitmap file : " << filename << "\n");
		Misc::breakExit(11);
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
	if (bmpHeader->bfType != 0x4D42) {
		D("File \"" << filename.c_str() << "\" isn't a bitmap file\n");
		Misc::breakExit(2);
	}
	else if (bmpInfo->biWidth < 0 || bmpInfo->biHeight < 0) {
		D("File \"" << filename.c_str() << "\" has a negative width or height. Not supported (yet).\n");
		Misc::breakExit(2);
	}
	GLenum bitMode = GL_RGB;
	int pixelSize = bmpInfo->biBitCount / 8;
	if (bmpInfo->biBitCount == 24) {
		bitMode = GL_RGB;
	}
	else if (bmpInfo->biBitCount == 32) {
		bitMode = GL_RGBA;
	}
	else {
		D("File \"" << filename.c_str() << "\" isn't a 24/32 bits bitmap file\n");
		Misc::breakExit(2);
	}

	int row_size_used = this->_width * pixelSize;
	int padding = (4 - (row_size_used % 4)) % 4;
	int row_size_full = row_size_used + padding;
	D("row_size_used:" << row_size_used << "\n");
	D("padding:" << padding << "\n");
	D("row_size_full:" << row_size_full << "\n");
	D("bmpInfo->biSize:" << bmpInfo->biSize << "\n");
	D("bmpInfo->biWidth:" << bmpInfo->biWidth << "\n");
	D("bmpInfo->biHeight:" << bmpInfo->biHeight << "\n");
	D("bmpInfo->biCompression:" << bmpInfo->biCompression << "\n");
	D("bmpInfo->biSizeImage:" << bmpInfo->biSizeImage << "\n");
	// if (!bmpInfo->biSizeImage)//Specifies the size, in bytes, of the image. This can be set to 0 for uncompressed RGB bitmaps.
	bmpInfo->biSizeImage = row_size_full * bmpInfo->biHeight;
	D("bmpInfo->biSizeImage:" << bmpInfo->biSizeImage << " (computed)" << "\n");
	D("bmpInfo->biBitCount:" << bmpInfo->biBitCount << "\n");

	unsigned int RGBsize = (this->_width * 3) * bmpInfo->biHeight;// 3 cauz RGB
	D("RGBsize:" << RGBsize << "\n");
	uint8_t* pixels = new uint8_t[bmpInfo->biSizeImage];
	// this->_data = new uint8_t[bmpInfo->biSizeImage];//oversized, but well padded for opengl (contains padding and potential 32bit size)
	this->_data = new uint8_t[RGBsize];//size can be not well padded for openGL, see Texture::loadToGPU();

	// Go to where image data starts, then read in image data
	file.seekg(bmpHeader->bfOffBits);
	file.read((char*)pixels, bmpInfo->biSizeImage);

	D("transfering...\n");
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
			// D("fuck A:" << (int)pixels[i] << "\n");
		}
		else if (padding && (i % row_size_full) > row_size_used - 1) {
			// D("fuck padding\n");
		}
		else {
			this->_data[j] = pixels[i];
			j++;
		}
	}

	/*
		.bmp files store image data in the BGR format, and we have to convert it to RGB.
		Just swap Red and Blue values.
		This can be avoided by using GL_BGR instead of GL_RGB with glTexImage2D() func
		note: the data is stocked as lines, from last line to 1st line
	*/
	D("converting BGR to RGB...");
	uint8_t tmp = 0;
	for (unsigned long i = 0; i < RGBsize; i += 3) {
		tmp = this->_data[i + 0];
		this->_data[i + 0] = this->_data[i + 2];
		this->_data[i + 2] = tmp;
	}
	D(" Done\n");
	//construct GL Texture
	this->loadToGPU();

	delete bmpHeader;
	delete bmpInfo;
	delete[] pixels;
	D(__PRETTY_FUNCTION__ << " END\n");
	D(D_SPACER_END << "\n");
}

Texture::Texture(uint8_t* data, unsigned int width, unsigned int height) : _width(width), _height(height) {
	this->_filename = "N/A (data)";
	int size = width * height * 3;
	this->_data = new uint8_t[size];
	memcpy(this->_data, data, size);
	this->_isLoaded = false;
	this->loadToGPU();
}

Texture::Texture(const Texture& src) {
	//D("_ Texture cons by copy" << "\n")
	*this = src;
}

Texture& Texture::operator=(const Texture& src) {
	this->_filename = src._filename;
	this->_width = src._width;
	this->_height = src._height;
	unsigned int	size = this->_width * this->_height * 3;
	this->_data = new uint8_t[size];
	memcpy(this->_data, src._data, size);
	this->_isLoaded = false;
	this->loadToGPU();
	return (*this);
}

Texture::~Texture() {
	//D(__PRETTY_FUNCTION__ << this << " : " << this->_filename << "\n")
	delete[] this->_data;
	this->unloadFromGPU();
}

void	Texture::updateData(uint8_t* data, unsigned int width, unsigned int height) {
	//force the user to send widht and height, adds a layer of security in case of unadapted data
	if (this->_width != width || this->_height != height) {
		D("Texture::updateData(...) failed: wrong width and/or height\n");
		D(this->_width << "x" << this->_height << " != " << width << "x" << height << "\n");
		D(this->_id << ": " << this->_filename << "\n");
		//Misc::breakExit(4);
	} else {
		memcpy(this->_data, data, width * height * 3);
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
			D("\tline " << (i + 3) / (this->_width * 3) << "\n");
		D((unsigned int)(this->_data[i]) << ":");
		if (i % 3 == 2)
			D("\n");
	}
}

void	Texture::loadToGPU() {
	if (this->_isLoaded) {
		D("warning: " << this->_filename << " is already loaded\n");
	} else {
		this->_isLoaded = true;
		//D(this->_id << "\n")
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
		//glGenerateTextureMipmap(this->_id); // (only gl4.5+) equivalent to glGenerateMipmap(GL_TEXTURE_2D); if this->_id is bound
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// GL_NEAREST GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_REPEAT GL_MIRRORED_REPEAT GL_CLAMP_TO_EDGE GL_CLAMP_TO_BORDER
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void	Texture::unloadFromGPU() {
	glDeleteTextures(1, &this->_id);
	this->_isLoaded = false;
	this->_id = -1;
}

//accessors
GLuint			Texture::getId() const { return (this->_id); }
std::string		Texture::getFilename() const { return (this->_filename); }
unsigned int	Texture::getWidth() const { return (this->_width); }
unsigned int	Texture::getHeight() const { return (this->_height); }
uint8_t*		Texture::getData() const { return (this->_data); }
bool			Texture::isLoaded() const { return (this->_isLoaded); }
