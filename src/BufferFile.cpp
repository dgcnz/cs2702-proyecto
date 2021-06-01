#include "BufferFile.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

BufferFile::BufferFile () {
	curr_key = false;

	to_read = -1;
	to_delete = -1;

	filename = new char [100000];
	mainfile = new char [100000];
}

void BufferFile::read_headers (std::fstream &file) {
	file.seekg(file.beg);

	file.read((char *) &to_read, sizeof(int));
	file.read((char *) &to_delete, sizeof(int));
}


void BufferFile::write_headers (std::fstream &file, int to_r, int to_d) {
	file.seekg(0);

	file.write((char *) &to_r, sizeof(int));
	file.write((char *) &to_d, sizeof(int));
}


void BufferFile::read_register (Register &reg) {
	file.read((char *) &reg.next_register, sizeof(int));
	file.read((char *) &reg.next_delete, sizeof(int));
	file.read((char *) &reg.pos_disk, sizeof(int));
	file.read((char *) &reg.key, sizeof(int));
}


int BufferFile::open (char *filename) {
	this->filename = filename;

	file.open (filename, std::ios::in | std::ios::out | std::ios::binary);
	
	if (!file.good()) return false;

	read_headers(file);

	return file.good();
}


int BufferFile::create(char *filename) {
	this->filename = filename;
	file.open ((const char *) filename, std::ios::out | std::ios::binary);

	if (!file.good()) {
		close();
		return -1;
	}

	write_headers(file, -1, -1);

	close();
	return 1;
}


int BufferFile::close() {
    file.close();
    file.clear();
    return true;
}

int BufferFile::read(int addr, int addr1) {
	if (addr == -1) return read_all();

	if (addr != -1 && addr1 == -1) return find(addr);

	if (addr != -1 && addr1 != -1) return find(addr, addr1);
	
	return 0;
}

int BufferFile::find(int addr) {
	int pos;
	file.seekg(0);

	if (file.eof()) return -1;

	file.read((char *) &pos, sizeof(int));
	file.seekg(pos);

	read_register(buffer);
	
	if (buffer.key == addr) {
	 	if (read_disk(buffer.pos_disk) == -1) {
			std::cout << "a\n";
			return -1;
		}

		return pos;
	}

	int i = 1;
	while (buffer.key != addr && buffer.next_register != -1) {
		pos = buffer.next_register;
		file.seekg(pos);

		read_register(buffer);

		if (buffer.next_delete == -1 && buffer.key == addr) {
			if (read_disk(buffer.pos_disk)  == -1) {
			std::cout << "b\n";
				return -1;
			}

			return pos;
		}

		i++;

		if (file.fail()) {
			file.clear();

			std::cout << "c\n";
			return -1;
		}
	}

	pos = buffer.next_register;
	file.seekg(pos);

	read_register(buffer);
	if (buffer.key == addr) {
		if (read_disk(buffer.pos_disk) == -1) {
			std::cout << "d\n";
			return -1;
		}
		
		return pos;
	}


	std::cout << "e\n";
	return -1;
}


int BufferFile::find(int addr, int addr2) {
	int pos;
	file.seekg(0);

	if (!file.good()) return -1;

	file.read((char *) &pos, sizeof(int));
	file.seekg(pos);

	read_register(buffer);
	
	if (buffer.key == addr) read_disk(buffer.pos_disk);

	while (buffer.key < addr2 && buffer.next_register != -1) {
		file.seekg(buffer.next_register);

		read_register(buffer);
	
		if (buffer.next_delete == -1 && buffer.key < addr2)
			read_disk(buffer.pos_disk);

		if (file.fail()) {
			file.clear();
			return -1;
		}
	}

	file.seekg(buffer.next_register);

	read_register(buffer);
	if (buffer.key >= addr && buffer.key <= addr2) return read_disk(buffer.pos_disk);

	return -1;
}



int BufferFile::read_all() {
	int pos, to_delete;

	file.seekg(file.beg);

	if (!file.good()) return -1;

	file.read((char *) &pos, sizeof(int));
	file.read((char *) &to_delete, sizeof(int));
	file.seekg(pos);

	read_register(buffer);
	if (buffer.next_delete == -1 && pos != to_delete) {
		read_disk(buffer.pos_disk);
}

	int i = 1;
	while (buffer.next_register != -1) {
		pos = buffer.next_register;
		file.seekg(buffer.next_register);

		read_register(buffer);

		if (buffer.next_delete == -1 && pos != to_delete)
			read_disk(buffer.pos_disk);

		i++;

		if (file.fail()) {
			file.clear();
			return -1;
		}
	}

	//if (pos != to_delete) read_disk(buffer.pos_disk);

	i++;

	if (file.fail()) {
		file.clear();
		return -1;
	}

	return 1;

}

int BufferFile::read_disk(int addr) {
	std::fstream main(mainfile, std::ios::in);
	std::string line;

	main.seekg(addr);
	if (main.good()) {
		std::getline (main, line);
		std::stringstream ss (line);
	
		std::cout << line << '\n';

		return 1;
	}

	return addr;
}

int BufferFile::insert (int key, int disk_addr) {
	int pos = -1;

	file.seekg(file.beg);

	if (file.eof()) return -1;
	
	if  (!curr_key) {
		pos = file.tellg();
	
		write_headers (file, -1, -1);

		pos = file.tellg();

		std::cout << "pos:" << file.tellg() << '\n';
		Register reg (key, disk_addr);
		
		file.write((char *) &reg, sizeof(reg));

		file.seekg(file.beg);
		file.write((char *) &pos, sizeof(pos));

		curr_key = key;
		return 1;
	}

	// read the min key
	read_headers (file);

	if (to_delete != -1) {
		file.seekg(to_delete);

		read_register(buffer);

		file.seekg(sizeof(int));
		file.write((char *) &buffer.next_delete, sizeof(int));
		file.seekg(to_delete);

		pos = to_delete;
	}

	else {
		file.seekg(0, std::ios::end);
		pos = file.tellg();
	}

	buffer = Register (key, disk_addr);
	file.write ((char *) &buffer, sizeof(buffer));

	file.seekg(to_read);
	read_register(buffer_less);

	if (key > buffer_less.key) return insert_more (pos, to_read, key, disk_addr);
	
	if (key < buffer_less.key) {
		buffer.next_register = to_read;
		
		file.seekg(pos);
		file.write((char *) &buffer, sizeof(buffer));

		file.seekg(0);
		file.write((char *) &pos, sizeof(int));

		return 1;
	}

	return 0;
}


int BufferFile::insert_more (int pos, int pos_less, int key, int disk_addr) {
	while (key > buffer_less.key && buffer_less.next_register != -1) {
		file.seekg(buffer_less.next_register);

		read_register(buffer_more);

		if (buffer_less.key < key && key < buffer_more.key) {
			//actualizo punteros
			buffer.next_register = buffer_less.next_register;
			buffer_less.next_register = pos;

			//escribo los cambios
			file.seekg(pos_less);
			file.write((char *) &buffer_less, sizeof(buffer_less));

			file.seekg(pos);
			file.write((char *) &buffer, sizeof(buffer));
			
			return 1;
		}

		pos_less = buffer_less.next_register;
		buffer_less = buffer_more;

		if (file.fail()) {
			file.clear();
			return -1;
		}
	}
	
	buffer_less.next_register = pos;

	file.seekg(pos_less);
	file.write((char *) &buffer_less, sizeof(buffer_less));
	file.seekg(pos_less);
	read_register (buffer);

	return 1;
}


int BufferFile::merge_file () {
	std::fstream aux ("index.aux", std::ios::out | std::ios::in | std::ios::binary);
	int pos, delete_pos, tmp_pos;

	file.seekg(0);
	file.read((char *) &pos, sizeof(int));
	tmp_pos = file.tellg();
	aux.write((char *) &tmp_pos, sizeof(int));
	
	file.read((char *) &delete_pos, sizeof(int));

	file.seekg(pos);
	read_register(buffer);

	int i = 1;
	while (buffer.next_register != -1) {
		tmp_pos = buffer.next_register;
		buffer.next_register = sizeof(buffer) * i + sizeof(int);
		if (buffer.next_delete == -1)
			aux.write((char *) &buffer, sizeof(buffer));

		file.seekg(tmp_pos);
		read_register(buffer);

		i++;

		if (file.fail()) {
			file.clear();
			return -1;
		}
	}

	tmp_pos = buffer.next_register;
	buffer.next_register = sizeof(buffer) * i + sizeof(int);
	aux.write((char *) &buffer, sizeof(buffer));

	file.seekg(tmp_pos);
	read_register(buffer);

	if (file.fail()) {
		file.clear();
		return -1;
	}

	int status = std::remove(filename);

	if (status != 0) return -1;

	status = std::rename("index.aux", filename);

	if (status != 0) return -1;

	return 1;
}


int BufferFile::remove(int key) {
	int pos = find(key);
	int to_delete = -2;
	
	if (pos == -1) {
		std::cout << "here error\n";
		return -1;
	}

	file.seekg(sizeof(int));
	file.read((char *) &to_delete, sizeof(int));

	if (to_delete == -1) {
		file.seekg(sizeof(int));
		file.write((char *) &pos, sizeof(int));

		return 1;
	}

	else {
		file.seekg(pos);
		read_register(buffer);
		buffer.next_delete = to_delete;
		file.write((char *) &buffer, sizeof(buffer));

		file.seekg(sizeof(int));
		file.write((char *) &pos, sizeof(pos));

		return 1;
	}

	return -1;
}

