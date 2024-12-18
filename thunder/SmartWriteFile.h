#pragma once
#include "SmartFile.h"

class SmartWriteFile : public SmartFile
{
	std::ofstream file;
public:
	~SmartWriteFile() override {
		if (file.is_open())
			file.close();
	}
	SmartWriteFile() {}
	void open(const std::string& fileName) override;
	void open(const char* fileName) override;
	void close();
	bool is_open() const override { return file.is_open(); }
	bool bad() const override { return file.bad(); }
	std::ofstream& getFile() override { return file; }
};