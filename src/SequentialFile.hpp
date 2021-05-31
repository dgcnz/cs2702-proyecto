#pragma once 

#include "BufferFile.hpp"

#include <iostream>
#include <vector>

class BufferFile;

template <int BLOCK_SIZE = 50> class SequentialFile {
public:
	SequentialFile ();

	int open (char *name);
	int create (char *name);
	int insert_file (char *name);
	int search (int key = -1, int key2 = -1);
	int remove (int key);

	std::vector<std::string> headers; 

protected:
	int insert (int key, int addr);

	char *filename;
	BufferFile *dir_file;

	friend class BufferFile;
};

