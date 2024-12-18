#pragma once
#include <stdio.h>
#include <fstream>
#include <string>

// this is an abstract class, smart file, which is used to open and close files. smartRead and smartWrite are derived from this class
class SmartFile
{
public:
	virtual ~SmartFile() = default;
	SmartFile& operator=(const SmartFile& other);
	SmartFile(const SmartFile&) = delete;
	SmartFile() {}
	virtual void open(const std::string& fileName)=0;
	virtual void open(const char* fileName)=0;
	virtual void close() = 0;
	virtual bool is_open() const = 0;
	virtual bool bad() const = 0;
	virtual std::ios& getFile()=0;
};

