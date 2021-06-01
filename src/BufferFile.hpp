#pragma once 

#include "SequentialFile.hpp"

#include <iostream>
#include <fstream>


class BufferFile {
public:
	struct Register {
		int next_register; // pos dentro del index.dat
		int next_delete;
		int pos_disk;
		int key;
	
		Register () {
			next_register = -1;
			next_delete = -1;
		}

		Register (int key, int pos) : key(key), pos_disk(pos) {
			next_register = -1;
			next_delete = -1;
		}
	};

	BufferFile ();

//protected:
	int open (char *filename);
	int create (char *filename);
	int read (int addr = -1, int addr2 = -1);
	int find (int addr);
	int find (int addr, int addr2);
	int read_all ();
	int insert (int key, int disk_addr);
	int merge_file ();
	int remove (int key);

	char *filename;
	char *mainfile;

protected:
	int write (int addr = -1);
	int read_disk (int addr);
	int insert_more (int pos, int pos_less, int key, int disk_addr, Register buffer);
	void read_register (Register &reg);

	int curr_key;

	Register buffer_more;
	Register buffer_less;
	Register buffer;

  std::fstream file;
};
