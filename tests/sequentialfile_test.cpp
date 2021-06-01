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
	
	sf.insert_file("test.csv");
	
}

