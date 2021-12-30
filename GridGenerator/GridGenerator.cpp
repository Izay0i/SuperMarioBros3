#include <Windows.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//const unsigned int SCREEN_WIDTH = 256;
//const unsigned int SCREEN_HEIGHT = 224;
const unsigned int SCREEN_WIDTH = 500;
const unsigned int SCREEN_HEIGHT = 500;

const unsigned int CELL_WIDTH = SCREEN_WIDTH;
const unsigned int CELL_HEIGHT = SCREEN_HEIGHT;
//const unsigned int CELL_WIDTH = static_cast<unsigned int>(SCREEN_WIDTH / 2);
//const unsigned int CELL_HEIGHT = static_cast<unsigned int>(SCREEN_HEIGHT / 2);

const unsigned int MAX_FILE_LINE = 5000;

bool wroteTagOnce = false;
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

    //Be sure to use std::flush or std::endl to flush the buffer
    outFile << "[GRIDCELLS]\n";
    outFile << xCells << '\t' << yCells << '\n';
    outFile << "[/]\n\n";
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

    if (!wroteTagOnce) {
        wroteTagOnce = true;
        
        outFile << "#objID" << '\t' << "Cell_X" << '\t' << "Cell_Y\n";
        outFile << "[POSITIONS]\n";
    }

    outFile << objectID << '\t' << cellPosX << '\t' << cellPosY << '\n';
}

int main() {
    std::string file = "stage_fortress.txt";
    std::ofstream outputFile("grid_stage_fortress.txt");
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

    //Add closing tag
    outputFile << "[/]" << std::flush;

    readFile.close();

    std::cout << xCells << '\t' << yCells << std::endl;

    /*unsigned int width = 240;
    unsigned int height = 32;
    unsigned int offset = 16;
    unsigned int starting_x = 1856;
    unsigned int starting_y = 752;
    char format[] = "231\t34\t247\t50\t%u\t%u\n";
    char debug[100];

    for (unsigned int i = 0; i < width; i += offset) {
        for (unsigned int j = 0; j < height; j += offset) {
            sprintf_s(debug, format, starting_x + i, starting_y + j);
            OutputDebugStringA(debug);
        }
    }*/

    return 0;
}