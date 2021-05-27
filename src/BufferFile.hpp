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

		Register () {
			key = new char [10000];
		}

		Register (char *key, int next_reg, int pos) : key(key), next_register(next_reg), pos_disk(pos) {}
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
	int insert (char *key, int disk_addr);

	char *buffer;
	int buffer_size;
  std::fstream file;
};
