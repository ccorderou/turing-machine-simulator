#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

/**
    @param: Takes in the current state and current symbol
    @post: Returns the new state, new type symbol, and direction in a string
*/
// std::string transitionFunction(std::string currentState, std::string currentTypeSymbol);

struct TuringMachine
{
    std::vector<std::string> states;
    std::string inputSymbols[2] = {"0", "1"};
    std::vector<std::string> tapeSymbols;
    std::unordered_map<std::string, std::string> transitionFunction;
    // std::string transitionFunction(std::string currentState, std::string currentTypeSymbol);
    std::string startState = "q0";
    std::string blankSymbol = "B";
    std::vector<std::string> acceptingStates;
    std::vector<std::string> tape;
};

// helper function to reduce redundancies
bool contains(const std::vector<std::string> container, const std::string &letter);

int main()
{

    // Prompt user for file name
    std::cout << "Enter a file name: ";
    std::string fileName{};
    std::getline(std::cin, fileName);
    // Instantiate file-reading object
    std::ifstream inputFile(fileName);

    // Each line is either:
    //  1) blank
    //  2) a comment indicated by "//"
    //  3) or a 5-tuple consisting of currentState currentTapeSymbol newState newTapeSymbol direction
    std::string fileLine{};
    TuringMachine simulator;
    bool isFiveTuple{false};
    while (std::getline(inputFile, fileLine))
    {
        // if a line begins with an integer, it must be a transition function
        if (std::isdigit(fileLine[0]))
            isFiveTuple = true;

        // /currentState currentTapeSymbol newState newTapeSymbol direction
        if (isFiveTuple)
        {
            if (!contains(simulator.states, std::string(1, fileLine[0])))
                simulator.states.push_back(std::string(1, fileLine[0]));
            if (!contains(simulator.tapeSymbols, std::string(1, fileLine[2])))
                simulator.tapeSymbols.push_back(std::string(1, fileLine[2]));

            std::string stateWithSymbol{};
            stateWithSymbol += fileLine[0];
            stateWithSymbol += fileLine[2];

            std::string move{};
            move += fileLine[4];
            move += fileLine[6];
            move += fileLine[8];
            simulator.transitionFunction[stateWithSymbol] = move;

            // reset isFiveTuple variable
            isFiveTuple = false;
        }
    }

    std::cout << "THE STATES ARE: " << std::endl;
    for (int i{0}; i < simulator.states.size(); i++)
    {
        std::cout << '\t' << simulator.states[i] << std::endl;
    }

    std::cout << std::endl;

    std::cout << "THE TAPE SYMBOLS ARE: " << std::endl;
    for (int i{0}; i < simulator.tapeSymbols.size(); i++)
    {
        std::cout << '\t' << simulator.tapeSymbols[i] << std::endl;
    }

    std::cout << std::endl;

    for (auto it{simulator.transitionFunction.begin()}; it != simulator.transitionFunction.end(); it++)
    {
        std::cout << "Key-Value Pair:" << std::endl;
        std::cout << '\t' << "Key:" << it->first;
        std::cout << '\t' << "Value:" << it->second;
        std::cout << std::endl;
    }

    std::cout << std::endl;

    std::cout << simulator.transitionFunction.size() << std::endl;

    return 0;
}

bool contains(const std::vector<std::string> container, const std::string &letter_val)
{
    for (const auto &letter : container)
        if (letter == letter_val)
            return true;
    return false;
}