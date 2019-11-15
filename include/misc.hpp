#pragma once

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <fstream>
#include <algorithm>

class Misc
{
public:
	static std::string		getCurrentDirectory(void);
	static std::string		getFileContent(const char *filename);
	static void				logfile(std::string basename, std::string logs);
	static std::string		crossPlatPath(std::string path);
	static void				breakExit(int err);
	static void				intToRGB(unsigned int value, uint8_t * dst);
	static void				RGBToInt(uint8_t * rgb, unsigned int * dst);
private:
	Misc();
	~Misc();
};

