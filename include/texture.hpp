#pragma once
#include "simplegl.h"
#include <iostream>
#include <fstream>

/*
	This class takes .bmp 24bits file (unknow behavior on other .bmp format)
	Code based on http://www.cplusplus.com/articles/GwvU7k9E/
		* corrected data padding behavior
		* corrected useless intermediate variables

	below structs to avoid Windows dependencys:
	https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/ns-wingdi-tagbitmapfileheader
	https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376(v=vs.85).aspx
*/

/*
	managing memory (replace + delete)
	https://stackoverflow.com/questions/11217121/how-to-manage-memory-with-texture-in-opengl
*/

#if defined(__APPLE__) || defined(__linux__) 
#define WORD	unsigned short	//2
#define DWORD	int				//4
#define LONG	int				//4
#endif

//avoid padding after bfType
#pragma pack(push, 2)
typedef struct tagBMPFILEHEADER {//size = 14
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BMPFILEHEADER;
typedef struct tagBMPINFOHEADER {//size = 40
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD	biCompression;
	DWORD	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
} BMPINFOHEADER;
#pragma pack(pop)


class Texture
{
#if UNIT_TESTS == true
	friend class UnitTests;
#endif
public:
	Texture(std::string filename);
	Texture(uint8_t* data, unsigned int width, unsigned int height);
	Texture(const Texture& src);
	Texture&	operator=(const Texture& src);
	~Texture();

	// width and height have to be the same, undefined behavior of data sent is not large enough
	void	updateData(uint8_t* data, unsigned int width, unsigned int height);
	void	loadTexture();//GPU memory
	void	unloadTexture();//GPU memory
	void	printData() const;

	//accessors
	GLuint			getId() const;
	std::string		getFilename() const;
	unsigned int	getWidth() const;
	unsigned int	getHeight() const;

	/*
		This array is used to build a texture in the GPU
		If you delete it, you will not be able to load the texture later
	*/
	uint8_t*		getData() const;
	bool			isLoaded() const;

private:
	bool			_isLoaded;//GC memory
	GLuint			_id;
	std::string		_filename;
	unsigned int	_width;
	unsigned int	_height;
	uint8_t*		_data;

};

