#pragma once 

#include "SequentialFile.hpp"

#include <iostream>
#include <fstream>


class BufferFile {
public:
	struct Register {
		int next_register; // pos dentro del index.dat
		int pos_disk;
		char *key;
	};

	BufferFile ();

//protected:
	int open (char *filename);
	int create (char *filename);
	int close ();
	int read (int addr = -1);
	int read_all ();
	int read_disk (int addr);
	int write (int addr = -1);

	char *buffer;
	int buffer_size;
  std::fstream file;
  int write_header();

};
