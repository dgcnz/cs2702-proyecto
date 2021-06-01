#include "SequentialFile.hpp"

#include <iostream>

int main() {
	SequentialFile sf;

	sf.insert_file("dataset.csv");
	//sf.search();
	
	std::cout << "\nsearch by key\n";
	sf.search(2);
	
	std::cout << "\nsearch by range\n";
	sf.search(6, 7);

	std::cout << "\n\ndelete by key\n";
	if (sf.remove(6)	== -1) std::cout << "error\n";

	std::cout << "new regs\n";
	sf.search(1, 6);

	return 0;
}
