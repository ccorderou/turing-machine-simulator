#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/**
    @param: Takes in the current state and current symbol
    @post: Returns the new state, new type symbol, and direction in a string
*/
std::string transitionFunction(std::string currentState, std::string currentTypeSymbol);

struct Components
{
    std::vector<std::string> states;
    std::vector<int> inputSymbols;
    std::vector<std::string> tapeSymbols;
    std::string transitionFunction(std::string currentState, std::string currentTypeSymbol);
    std::string startState = "q1";
    std::string blankSymbol = "B";
    std::vector<std::string> acceptingStates;
};

int main()
{

    // Prompt user for file name
    std::cout << "Enter a file name: ";
    std::string fileName{};
    std::getline(std::cin, fileName);
    // Instantiate file-reading object
    std::ifstream inputFile(fileName);

    std::string fileLine{};
    while (std::getline(inputFile, fileLine))
    {
        std::cout << fileLine << std::endl;
    }
    return 0;
}