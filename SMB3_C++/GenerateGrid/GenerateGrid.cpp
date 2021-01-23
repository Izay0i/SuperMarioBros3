#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../SMB3_C++/assets/headers/Util.h"

const int MAX_FILE_LINE = 3000;
const int CELL_WIDTH = Util::SCREEN_WIDTH / 2;
const int CELL_HEIGHT = Util::SCREEN_HEIGHT / 2;

int xCells, yCells;

enum class SceneSection {
	SCENE_FILE_SECTION_UNKNOWN,
	SCENE_FILE_SECTION_MAPSIZE,
	SCENE_FILE_SECTION_CAMERALIMITS,
	SCENE_FILE_SECTION_TIME,
	SCENE_FILE_SECTION_BGCOLOR,
	SCENE_FILE_SECTION_TEXTURES,
	SCENE_FILE_SECTION_HUD,
	SCENE_FILE_SECTION_GRID,
	SCENE_FILE_SECTION_ENTITYDATA,
	SCENE_FILE_SECTION_TILESDATA,
	SCENE_FILE_SECTION_TILESPRITES,
	SCENE_FILE_SECTION_BACKGROUND
};

void ParseMapSize(std::ofstream& outFile, std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 2) {
		return;
	}

	xCells = std::stoi(tokens.at(0));
	yCells = std::stoi(tokens.at(1));
	
	outFile << xCells << '\t' << yCells << std::endl;
}

void ParseEntityData(std::ofstream& outFile, std::string line) {
	std::vector<std::string> tokens = Util::split(line);

	if (tokens.size() < 5) {
		return;
	}

	float posX = std::stof(tokens.at(3));
	float posY = std::stof(tokens.at(4));

	int cellX = static_cast<int>(posX / CELL_WIDTH);
	int cellY = static_cast<int>(posY / CELL_HEIGHT);

	if (cellX < 0) {
		cellX = 0;
	}
	else if (cellX >= xCells) {
		cellX = xCells - 1;
	}

	if (cellY < 0) {
		cellY = 0;
	}
	else if (cellY >= yCells) {
		cellY = yCells - 1;
	}

	std::string cellXStr = std::to_string(cellX);
	std::string cellYStr = std::to_string(cellY);

	outFile << cellXStr << '\t' << cellYStr << '\t';

	for (unsigned int i = 0; i < tokens.size() - 1; ++i) {
		outFile << tokens.at(i) << '\t';
	}

	outFile << tokens.back();
	outFile << std::endl;
}

int main() {
	std::string filePath = "stage_one.txt";
	std::ofstream outputFile("stage_one_grid.txt");
	outputFile.clear();

	std::ifstream inFile;
	inFile.open(filePath, std::ios::in);

	if (!inFile.is_open()) {
		OutputDebugStringA("[GRID GENERATOR] Failed to read file\n");
		return -1;
	}

	SceneSection section = SceneSection::SCENE_FILE_SECTION_UNKNOWN;

	char str[MAX_FILE_LINE];
	while (inFile.getline(str, MAX_FILE_LINE)) {
		std::string line(str);

		if (line.empty() || line.front() == '#') {
			continue;
		}

		if (line == "[LIMITS]" ||
			line == "[TIME]" ||
			line == "[BGCOLOR]" ||
			line == "[TEXTURES]" ||
			line == "[HUD]" ||
			//line == "[TILES_DATA]" ||
			line == "[TILE_SPRITES_ARR]" ||
			line == "[BACKGROUND]")
		{
			section = SceneSection::SCENE_FILE_SECTION_UNKNOWN;
			continue;
		}

		if (line == "[MAPSIZE]") {
			section = SceneSection::SCENE_FILE_SECTION_MAPSIZE;
			continue;
		}

		if (line == "[ENTITY_DATA]") {
			section = SceneSection::SCENE_FILE_SECTION_ENTITYDATA;
			continue;
		}

		switch (section) {
			case SceneSection::SCENE_FILE_SECTION_MAPSIZE:
				ParseMapSize(outputFile, line);
				break;
			case SceneSection::SCENE_FILE_SECTION_ENTITYDATA:
				ParseEntityData(outputFile, line);
				break;
		}
	}

	inFile.close();

	std::cout << xCells / CELL_WIDTH << '\t' << yCells / CELL_HEIGHT << std::endl;

	return 0;
}