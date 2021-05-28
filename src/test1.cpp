#include "SequentialFile.hpp"

int main() {
	SequentialFile sf;

	int result;
	result = sf.create("hi");

	if (result == -1) {
		std::cout << "error creating\n";
		return 0;
	}

	sf.insert_file("test.csv");

	return 0;
}
