#include "SmartWriteFile.h"

#include <string>


void SmartWriteFile::close() {
	if (file.is_open()) {
		file.close();
	}
}

SmartWriteFile& SmartWriteFile::operator=(const SmartWriteFile& other)
{
	if (this != &other)
	{
		if (other.file.is_open())
		{
			throw std::exception("cannot assign with opened file");
		}
		if (file.is_open())
		{
			file.close();
		}
	}
	return *this;
}

void SmartWriteFile::open(const std::string& fileName)
{
	if (file.is_open())
		throw std::exception("shouldnt open opened file");
	file.open(fileName, std::ios::out | std::ios::trunc);
	if ((not file.is_open()) or file.bad())
		throw std::exception("could not open the file");
}

void SmartWriteFile::open(const char* fileName)
{
	open((std::string)(fileName));
}