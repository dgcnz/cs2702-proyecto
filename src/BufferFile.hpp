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
	int close ();
	int read (int addr = -1, int addr2 = -1);
	int find (int addr);
	int find (int addr, int addr2);
	int read_all ();
	int read_disk (int addr);
	int write (int addr = -1);
	int insert (int key, int disk_addr);
	int insert_more (int pos, int pos_less, int key, int disk_addr, Register buffer);
	int merge_file ();
	int remove (int key);

	void read_register (Register &reg);

	int curr_key;
	Register buffer_more;
	Register buffer_less;
	Register buffer;

	char *filename;
	char *mainfile;
  std::fstream file;
};
