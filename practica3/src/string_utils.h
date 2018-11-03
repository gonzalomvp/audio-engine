#pragma once

#include <sstream>

template <typename T> std::string stringFromNumber(T val) {
	std::ostringstream stream;
	stream << std::fixed << val;
	return stream.str();
}

inline std::string extractPath(const std::string& filename) {
	std::string filenameCopy = filename;
	while (filenameCopy.find("\\") != std::string::npos) {
		filenameCopy.replace(filenameCopy.find("\\"), 1, "/");
	}
	filenameCopy = filenameCopy.substr(0, filenameCopy.rfind('/'));
	if (filenameCopy.size() > 0) filenameCopy += "/";
	return filenameCopy;
}

inline std::string extractExt(const std::string& filename) {
	return filename.substr(
		filename.rfind('.') + 1,
		filename.length() - filename.rfind('.') - 1);
}