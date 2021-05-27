#include "SequentialFile.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

template<>
SequentialFile<50>::SequentialFile () {
	filename = new char;
	dir_file = new BufferFile;
}

void make_names (char *name, char *&dir_name) {
	std::ostringstream tmp;
	tmp << name << ".dir" << std::ends;
	dir_name = strdup(tmp.str().c_str());
}

template <>
int SequentialFile<>::open (char *name) {
	char *dir_name;
	int result;

	make_names(name, dir_name);

	filename = dir_name;

	//result = dir_file->open(dir_name);

	if (!result) return 0;

	//result = dir_file->read();

	if (result == -1) return 0;

	return result;
}



template <>
int SequentialFile<>::create(char *name) {
	char *dir_name;

	make_names(name, dir_name);

	int result = dir_file->create(dir_name);
	
	if (!result) return 0;

	filename = dir_name;

	return result;
}

template<>
int SequentialFile<>::insert(char *key, int addr) {
	//int pos = search(key);

	//if (search(key) != -1) return dir_file->insert(key, addr);
	dir_file->insert(key, addr);

	return -1;
}

template <>
int SequentialFile<>::insert_file(char *name) {

	std::fstream file (name, std::ios::in | std::ios::binary);

	std::string line, colname;

	// read and save the headers
	if (file.good()) {	
		std::getline (file, line);
		std::stringstream ss (line);

		while (std::getline (ss, colname, ','))
			headers.push_back(colname);
	}

	std::istream::pos_type pos = file.tellg();
	while (std::getline(file, line)) {
		std::stringstream ss(line);

		char *writable = new char [10000];
		char tmp = line[0];
		writable = &tmp;

		insert(writable, pos);

		pos = file.tellg();
	}
		
		file.clear();
	
		file.seekg(13, file.beg);
		char *h = new char[1000];
		file.read(h, 1000);

		return 1;
}


template<>
int SequentialFile<>::search(char *key) {
	/*int bucket_addr = find(key);

	load_bucket(current_bucket, bucket_addr);

	return dir_file->search(key);*/
}

