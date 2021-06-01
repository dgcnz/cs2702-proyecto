#pragma once 

#include "SequentialFile.hpp"

#include <iostream>
#include <fstream>


class BufferFile {
public:
	BufferFile ();

//protected:
	int open (char *filename);
	int create (char *filename);
	int close ();
	int read (int addr = -1, int addr2 = -1);
	int find (int addr, bool del = 0);
	int find (int addr, int addr2);
	int read_all ();
	int insert (int key, int disk_addr);
	int merge_file ();
	int remove (int key);

	char *filename;
	char *mainfile;

protected:
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

	void read_headers (std::fstream &file);
	void write_headers (std::fstream &file, int to_r, int to_d);
	int read_disk (int addr, bool del = 0);
	int insert_more (int pos, int pos_less, int key, int disk_addr);
	void read_register (Register &reg);

	bool curr_key;
	int to_read;
	int to_delete;

	Register buffer_more;
	Register buffer_less;
	Register buffer;

  std::fstream file;
};
