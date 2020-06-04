#include "simplegl.h"
#include "misc.hpp"

Misc::Misc() {
}


Misc::~Misc() {
}

std::string			Misc::getCurrentDirectory(void) {
	char	path[FILENAME_MAX];

	memset(path, 0, sizeof(path));
	if (getcwd(path, sizeof(path)) == nullptr) {
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

	if (fp) {
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
	} else {
		cerr << "fopen failed to open : " << filename << endl;
		exit(102);
	}

	return (contents);
}

void				Misc::logfile(std::string basename, std::string logs) {
	std::replace(basename.begin(), basename.end(), '\\', '-');
	std::replace(basename.begin(), basename.end(), '/', '-');
	std::replace(basename.begin(), basename.end(), ':', '-');
	string path("log/" + basename + "_date" + ".log");
	path = Misc::crossPlatPath(path);
	//cout << "*** log file:\t\t" << path << endl;
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

void				Misc::intToRGB(unsigned int value, uint8_t * dst) {
	unsigned char * p = reinterpret_cast<unsigned char *>(&value);
	dst[0] = p[0];
	dst[1] = p[1];
	dst[2] = p[2];
}
void				Misc::RGBToInt(uint8_t * rgb, unsigned int * dst) {
	*dst = 0;
	unsigned char * p = reinterpret_cast<unsigned char *>(dst);
	p[0] = rgb[0];
	p[1] = rgb[1];
	p[2] = rgb[2];
}
