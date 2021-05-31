#include "SequentialFile.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void make_names (char *name, char *&dir_name) {
	std::ostringstream tmp;
	tmp << name << ".dat" << std::ends;
	dir_name = strdup(tmp.str().c_str());
}

template <>
int SequentialFile<>::create(char *name) {
	char *dir_name;

	make_names(name, dir_name);

	int result = dir_file->create(dir_name);
	
	if (result == -1) return -1;

	filename = dir_name;

	return result;
}

template<>
SequentialFile<>::SequentialFile () {
	filename = new char;
	dir_file = new BufferFile;
	
	int result = create("data");
	
	if (result == -1) {
		std::cout << "error creating\n";
	}
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





template<>
int SequentialFile<>::insert(int key, int addr) {
	//int pos = search(key);

	//if (search(key) != -1) return dir_file->insert(key, addr);
	dir_file->insert(key, addr);

	return -1;
}

template <>
int SequentialFile<>::insert_file(char *name) {
	dir_file->mainfile = name;
	std::fstream file (name, std::ios::in | std::ios::binary);

	std::string line, colname, id;

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

		std::getline (ss, id, ',');

		insert(std::stoi(id), pos);

		pos = file.tellg();
	}
		
		dir_file->merge_file();
		file.clear();
	
		return 1;
}


template<>
int SequentialFile<>::search(int key, int key2) {
	if (key == -1) return dir_file->read();

	if (key != -1 && key2 == -1) return dir_file->read(key);

	if (key != -1 && key2 != -1) return dir_file->read(key, key2); 

	return -1;
}

template<>
int SequentialFile<>::remove(int key) {
	return dir_file->remove(key);

	return 0;
}
