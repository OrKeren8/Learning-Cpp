#pragma once
#include "SmartFile.h"


class smartReadFile: public SmartFile {
	std::ifstream file;
public:
	~smartReadFile() override
	{
		if (file.is_open())
			file.close();
	}
	smartReadFile() {}  
	void open(const std::string& fileName) override;
	void open(const char* fileName) override;
	virtual void close();
	bool is_open() const override { return file.is_open(); }
	bool bad() const override { return file.bad(); }
	std::ifstream& getFile() override { return file; }
};