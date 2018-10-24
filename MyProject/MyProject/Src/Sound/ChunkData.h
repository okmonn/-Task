#pragma once
#include <string>

// RIFF
struct RIFF {
	unsigned char id[4];
	long size;
	unsigned char type[4];
};

// FMT
struct FMT {
	unsigned char id[4];
	long size;
	short type;
	short channel;
	long sample;
	long byte;
	short block;
	short bit;
};

// DATA
struct DATA {
	std::string id;
	long size;
};