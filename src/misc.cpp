#include "simplegl.h"
#include "misc.hpp"

Misc::Misc() {
}


Misc::~Misc() {
}

std::string			Misc::getCurrentDirectory(void) {
	char	path[FILENAME_MAX];

	memset(path, 0, sizeof(path));
	if (getcwd(path, sizeof(path)) == NULL)
	{
		cerr << "getcwd error" << endl;
		exit(UNKNOW_ERROR);
	}
	return (std::string(path));
}

#ifdef _WIN32 
static std::FILE*	windows_fopen_s(const char *filename, const char *mode) {
	std::FILE	*fp;
	fopen_s(&fp, filename, mode);
	return (fp);
}
#endif

std::string			Misc::getFileContent(const char *filename) {
	std::string	contents;
	std::FILE	*fp = fopen(filename, "rb");

	if (fp)
	{
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
		return(contents);
	}
	cerr << "fopen failed to open : " << filename << endl;

	exit(102);
	return (NULL);
}

void				Misc::logfile(std::string basename, std::string logs) {
	std::replace(basename.begin(), basename.end(), '\\', '-');
	std::replace(basename.begin(), basename.end(), '/', '-');
	std::replace(basename.begin(), basename.end(), ':', '-');
	string path("log/" + basename + "_date" + ".txt");
	path = Misc::crossPlatPath(path);
	cout << "*** log file:\t\t" << path << endl;
	ofstream file;
	file.open(path);
	file << logs;
	file.close();
}

std::string			Misc::crossPlatPath(std::string path) {
#ifdef _WIN32
	std::replace(path.begin(), path.end(), '/', '\\');
#else//__APPLE__   __linux__
	std::replace(path.begin(), path.end(), '\\', '/');
#endif
	return (path);
}

void				Misc::breakExit(int err) {
	cerr << "BREAKPOINT BEFORE EXIT : " << err << endl;

	exit(err);
}