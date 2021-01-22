#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../SMB3_C++/assets/headers/Util.h"

const int MAX_FILE_LINE = 3000;
const int CELL_WIDTH = 132;
const int CELL_HEIGHT = 132;

int xCells, yCells;

void ParseEntityData(std::ofstream& outFile, std::string line) {
	std::vector<std::string> tokens = Util::split(line);
}

int main() {
	std::string filePath = "map.txt";
	std::ofstream outputFile("map_grid.txt");
	outputFile.clear();

	std::ifstream inFile;
	inFile.open(filePath);

	char str[MAX_FILE_LINE];
	while (inFile.getline(str, MAX_FILE_LINE)) {

	}

	inFile.close();
}