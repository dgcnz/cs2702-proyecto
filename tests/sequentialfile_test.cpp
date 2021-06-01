#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SequentialFile.hpp>
#include <BufferFile.hpp>

#include <fmt/core.h>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

struct SequentialFileTests : public::testing::Test {};

TEST_F (SequentialFileTests, IndexingFile100) {
	SequentialFile sf;
	
	int result = sf.insert_file("t100.csv");
	
	EXPECT_EQ (result, 1);
}

TEST_F (SequentialFileTests, IndexingFile1000) {
	SequentialFile sf;
	
	int result = sf.insert_file("t1000.csv");
	
	EXPECT_EQ (result, 1);
}

TEST_F (SequentialFileTests, IndexingFile10000) {
	SequentialFile sf;
	
	int result = sf.insert_file("t1000.csv");
	
	EXPECT_EQ (result, 1);
}

TEST_F (SequentialFileTests, Search) {
	SequentialFile sf;
	
	sf.insert_file("t100.csv");

	srand (time(NULL));
	int num = rand() % 100;

	int result = sf.search(num);
	
	EXPECT_EQ (result, 1);
}

TEST_F (SequentialFileTests, RangeSearch) {
	SequentialFile sf;
	
	sf.insert_file("t100.csv");

	srand (time(NULL));
	int num = rand() % 100;

	int result = sf.search(num, num + 20);
	
	EXPECT_EQ (result, 1);
}


TEST_F (SequentialFileTests, Delete) {
	SequentialFile sf;
	
	sf.insert_file("t100.csv");

	srand (time(NULL));
	int num = rand() % 100;

	int result = sf.remove(num);
	
	EXPECT_EQ (result, 1);
}


