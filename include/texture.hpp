#pragma once
#include "simplegl.h"
#include <iostream>
#include <fstream>

/*
	This class takes .bmp 24bits files (unknow behavior on other .bmp format)
	Code based on http://www.cplusplus.com/articles/GwvU7k9E/
		* corrected padding behavior
		* corrected useless intermediate variables

	below structs to avoid Windows dependencys:
	https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/ns-wingdi-tagbitmapfileheader
	https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376(v=vs.85).aspx
*/

#ifdef __APPLE__
#define WORD	unsigned short
#define DWORD	unsigned long
#define LONG	long
#endif

#pragma pack(push, 2)//avoid padding after bfType
typedef struct tagBMPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BMPFILEHEADER;
typedef struct tagBMPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BMPINFOHEADER;
#pragma pack(pop)


class Texture
{
public:
	Texture(std::string filename);
	Texture(uint8_t* data, unsigned int width, unsigned int height);
	~Texture();

	void	printData() const;

	//accessors
	GLuint			getId() const;
	std::string		getFilename() const;
	unsigned int	getWidth() const;
	unsigned int	getHeight() const;
	uint8_t*		getData() const;//dangerous

private:
	GLuint			_id;
	std::string		_filename;
	unsigned int	_width;
	unsigned int	_height;
	uint8_t*		_data;

	void			genTexture();
};

