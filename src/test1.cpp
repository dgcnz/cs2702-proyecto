#include "SequentialFile.hpp"

#include <iostream>

int main() {
	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	SequentialFile sf;

	auto t1 = high_resolution_clock::now();
	sf.insert_file("dataset.csv");
	auto t2 = high_resolution_clock::now();

	duration<double, std::milli> ms_double = t2 - t1;
	std::cout << "insert :" << ms_double.count() << "ms";
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
