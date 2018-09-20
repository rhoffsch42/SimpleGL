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
	Misc();
	~Misc();

	static std::string		getCurrentDirectory(void);
	static std::string		getFileContent(const char *filename);
	static void				logfile(std::string basename, std::string logs);
	static std::string		crossPlatPath(std::string path);
	static void				breakExit(int err);
};

