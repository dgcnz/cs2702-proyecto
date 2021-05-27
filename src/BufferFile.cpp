#include "BufferFile.hpp"

#include <iostream>
#include <fstream>

BufferFile::BufferFile () {
	buffer = new char [1000000];
	buffer_size = 0;
}


int BufferFile::open (char *filename) {
	file.open (filename, std::ios::in | std::ios::out | std::ios::binary);

	if (!file.good()) return false;

	// get to the beggining of the file
	file.seekg(0, std::ios::beg);
	file.seekp(0, std::ios::beg);

	return file.good();
}

int BufferFile::create(char *filename) {
	file.open ((const char *) filename, std::ios::binary);

	if (!file.good()) {
		file.close();
		file.clear();
		return false;
	}

	return 1;
}

int BufferFile::read(int addr) {
	if (addr == -1) return read_all();
	else return read_disk(addr);
}

int BufferFile::read_all() {
	
}

int BufferFile::read_disk(int addr) {
	if (file.eof()) return -1;

	//int addr = file.tellg();
	unsigned short bff_size;

	//clear();

	file.read((char *) &bff_size, sizeof(bff_size));

	if (file.fail()) {
		file.clear();
		return -1;
	}

	buffer_size = bff_size;

	//if (buffer_size > size) return -1;

	file.read(buffer, buffer_size);

	if (file.fail()) {
		file.clear();
		return -1;
	}

	return addr;
}

int BufferFile::insert (char *key, int disk_addr) {
	if (file.eof()) return -1;

	char *current_key = new char [10000];
	int pos = file.tellg();

	file.read((char *) current_key, sizeof(10000));

	if (current_key != nullptr && current_key[0] == '\0') {
		Register reg (key, -1, disk_addr);

		file.write((char *) &key, 10000);
		file.write((char *) &reg, sizeof(reg));

		std::cout << "hi\n";
		return 1;
	}

	while (key > current_key) {
		std::cout << "here\n";
		file.read((char *) current_key, sizeof(10000));
		file.read((char *) buffer, sizeof(int) * 2);

		std::cout << buffer;

		pos = file.tellg();
		if (file.fail()) {
			file.clear();
			return -1;
		}
	}

	
}
