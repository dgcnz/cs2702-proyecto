#include "SequentialFile.hpp"

int main() {
	SequentialFile sf;

	sf.create("hi");
	sf.insert_file("test.csv");

	return 0;
}
