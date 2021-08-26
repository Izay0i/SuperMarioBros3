#include <iostream>
#include <fstream>
#include <vector>
#include <string>

const unsigned int SCREEN_WIDTH = 270;
const unsigned int SCREEN_HEIGHT = 264;

const unsigned int CELL_WIDTH = static_cast<int>(SCREEN_WIDTH / 2);
const unsigned int CELL_HEIGHT = static_cast<int>(SCREEN_HEIGHT / 2);

const int MAX_FILE_LINE = 5000;

bool writtenTagOnce = false;
unsigned int xCells = 0, yCells = 0;

enum class GridFileSection {
    GRIDFILE_SECTION_UNKNOWN,
    GRIDFILE_SECTION_SCENESIZE,
    GRIDFILE_SECTION_ENTITYDATA
};

std::vector<std::string> SplitStr(std::string line, std::string delimeter = "\t") {
    std::vector<std::string> tokens;
    size_t last = 0, next = 0;

    while ((next = line.find(delimeter, last)) != std::string::npos) {
        tokens.push_back(line.substr(last, next - last));
        last = next + 1;
    }

    tokens.push_back(line.substr(last));

    return tokens;
}

void ParseSceneSize(std::ofstream& outFile, std::string line) {
    std::vector<std::string> tokens = SplitStr(line);

    if (tokens.size() < 2) {
        return;
    }

    unsigned int sceneWidth = std::stoul(tokens.at(0));
    unsigned int sceneHeight = std::stoul(tokens.at(1));
    //Do float division and then ceil the value to get an extra row and column offscreen
    //Then cast it to unsigned int to truncate the decimals
    xCells = static_cast<unsigned int>(ceil((static_cast<float>(sceneWidth) / CELL_WIDTH)));
    yCells = static_cast<unsigned int>(ceil((static_cast<float>(sceneHeight) / CELL_HEIGHT)));

    //Be sure to use std::endl to flush the buffer
    outFile << "[GRIDCELLS]" << std::endl;
    outFile << xCells << '\t' << yCells << std::endl;
    outFile << "[/]\n" << std::endl;
}

void ParseEntityData(std::ofstream& outFile, std::string line) {
    std::vector<std::string> tokens = SplitStr(line);

    if (tokens.size() < 5) {
        return;
    }

    float posX = std::stof(tokens.at(3));
    float posY = std::stof(tokens.at(4));

    unsigned int objectID = std::stoul(tokens.at(0));

    //Convert world space to cell space
    unsigned int cellPosX = static_cast<unsigned int>(posX / CELL_WIDTH);
    unsigned int cellPosY = static_cast<unsigned int>(posY / CELL_HEIGHT);

    if (cellPosX < 0) {
        cellPosX = 0;
    }
    else if (cellPosX >= xCells) {
        cellPosX = xCells - 1;
    }

    if (cellPosY < 0) {
        cellPosY = 0;
    }
    else if (cellPosY >= yCells) {
        cellPosY = yCells - 1;
    }

    if (!writtenTagOnce) {
        writtenTagOnce = true;
        
        outFile << "#Cell_X" << '\t' << "Cell_Y" << '\t' << "objID" << std::endl;
        outFile << "[POSITIONS]" << std::endl;
    }

    outFile << cellPosX << '\t' << cellPosY << '\t' << objectID << std::endl;
}

int main() {
    std::string file = "stage_one.txt";
    std::ofstream outputFile("grid_stage_one.txt");
    outputFile.clear();

    std::ifstream readFile;
    readFile.open(file, std::ios::in);

    if (!readFile.is_open()) {
        std::cout << "[GRID GENERATOR] Failed to read file\n";
        return -1;
    }

    GridFileSection gridFileSection = GridFileSection::GRIDFILE_SECTION_UNKNOWN;

    char str[MAX_FILE_LINE];
    while (readFile.getline(str, MAX_FILE_LINE)) {
        std::string line(str);

        if (line.empty() || line.front() == '#') {
            continue;
        }

        if (line == "[/]") {
            gridFileSection = GridFileSection::GRIDFILE_SECTION_UNKNOWN;
            continue;
        }

        if (line == "[SCENESIZE]") {
            gridFileSection = GridFileSection::GRIDFILE_SECTION_SCENESIZE;
            continue;
        }

        if (line == "[ENTITYDATA]") {
            gridFileSection = GridFileSection::GRIDFILE_SECTION_ENTITYDATA;
            continue;
        }

        switch (gridFileSection) {
            case GridFileSection::GRIDFILE_SECTION_SCENESIZE:
                ParseSceneSize(outputFile, line);
                break;
            case GridFileSection::GRIDFILE_SECTION_ENTITYDATA:
                ParseEntityData(outputFile, line);
                break;
        }
    }

    //Closing tag
    outputFile << "[/]" << std::endl;

    readFile.close();

    std::cout << xCells << '\t' << yCells << std::endl;

    return 0;
}