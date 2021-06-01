#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <SequentialFile.hpp>
#include <BufferFile.hpp>

#include <fmt/core.h>

#include <iostream>
#include <string>

struct SequentialFileTests : public::testing::Test {};

TEST_F (SequentialFileTests, IndexingFile100) {
	SequentialFile sf;
	
	int result = sf.insert_file("dataset.csv");
	
	EXPECT_EQ (result, 1);
}

TEST_F (SequentialFileTests, Search) {
	SequentialFile sf;
	
	sf.insert_file("dataset.csv");

	int result = sf.search(134);
	
	EXPECT_EQ (result, 1);
}

TEST_F (SequentialFileTests, RangeSearch) {
	SequentialFile sf;
	
	sf.insert_file("dataset.csv");

	int result = sf.search(10, 24);
	
	EXPECT_EQ (result, 1);
}


TEST_F (SequentialFileTests, Delete) {
	SequentialFile sf;
	
	sf.insert_file("dataset.csv");

	int result = sf.remove(134);

	sf.search (100, 140);
	
	EXPECT_EQ (result, 1);
}


