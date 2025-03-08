#include <iostream>
#include <fstream>
#include <string>

int main(){

    //Prompt user for file name
    std::cout << "Enter a file name: ";
    std::string fileName{};
    std::getline(std::cin, fileName);
    //Instantiate file-reading object
    std::ifstream inputFile(fileName);

    std::string fileLine{};
    while( std::getline(inputFile, fileLine)){
        std::cout << fileLine << std::endl;
    }
    return 0;
}