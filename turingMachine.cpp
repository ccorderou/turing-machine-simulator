#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <list>

/**
    @param: Takes in the current state and current symbol
    @post: Returns the new state, new type symbol, and direction in a string
*/
// std::string transitionFunction(std::string currentState, std::string currentTypeSymbol);

struct TuringMachine
{
    std::vector<std::string> states{"f"};
    std::string inputSymbols[2] = {"0", "1"};
    std::vector<std::string> tapeSymbols;
    std::unordered_map<std::string, std::string> transitionFunction;
    // std::string transitionFunction(std::string currentState, std::string currentTypeSymbol);
    // std::string startState = "q0";
    // std::string blankSymbol = "B";
    // std::vector<std::string> acceptingStates;
    std::list<std::string> tape;
    // helper function to print contents of simulator for debugging purposes
    void display();
};

// helper function to reduce redundancies
bool contains(const std::vector<std::string> container, const std::string &letter);

// helper function to output IDs
void printInstantaneousDescription(std::list<std::string>::iterator start, std::list<std::string>::iterator end, std::list<std::string>::iterator curr, std::string currentState);

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
    std::cout << "Enter an input word: ";
    std::string userInput{};
    // Use getline to avoid potential cin issues with '\n' when pressing 'Enter' if prompted once more in the future.
    std::getline(std::cin, userInput);

    bool repromptInput{false};
    for (const auto &letter : userInput)
        if (letter != '0' && letter != '1')
            repromptInput = true;

    while (repromptInput)
    {
        std::cout << "Input must only contain 0s and/or 1s: ";
        std::string userInput{};
        // Use getline to avoid potential cin issues with '\n' when pressing 'Enter' if prompted once more in the future.
        std::getline(std::cin, userInput);
        repromptInput = false;
        for (const auto &letter : userInput)
            if (letter != '0' && letter != '1')
                repromptInput = true;
    }

    std::string blank{"B"};

    // I am going to place five preceding blanks in the front
    // and five trailing blanks in the back
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    for (const auto &letter : userInput)
        simulator.tape.push_back(std::string(1, letter));
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);
    simulator.tape.push_back(blank);

    // The finite control will begin at the start of the tape
    auto finiteControl = simulator.tape.begin();

    // We will readjust it so that it points to the first input seen
    while (*finiteControl == "B")
        finiteControl++;

    // std::cout << "BEGINNING WITH THE FIRST INPUT SYMBOL: " << *finite_control << std::endl;
    std::string currentState{"0"};
    bool isAccepting{false};
    bool isHalting{false};
    auto startingPosition = finiteControl;
    auto lastPosition = finiteControl;

    while (*lastPosition != "B")
        lastPosition++;
    // testLast--;

    auto initialLastPosition = lastPosition;
    int readjustAmountBack{0};
    // int readjustAmountFront{0};
    int position{0};
    // Declare a move variable for the transition function
    std::string direction;
    bool inTheNegatives{false};
    while (!isAccepting && !isHalting)
    {
        if (!direction.empty() && direction == "L" && position < 0)
        {
            startingPosition--;
            inTheNegatives = true;
        }
        else if (!direction.empty() && direction != "R" && position < 0)
        {
            startingPosition++;
            inTheNegatives = true;
        }
        // to avoid an extra blank on the front end to be printed
        else if (!direction.empty() && direction != "R" && position == 0 && inTheNegatives)
        {
            startingPosition++;
            inTheNegatives = false;
        }
        // if the finiteControl points to same cell, then testLast increments by one for printing purposes
        if (lastPosition == finiteControl)
        {
            lastPosition++;
            readjustAmountBack++;
        }
        // My finiteControl moved back once and so I readjust accordingly
        else if (lastPosition != finiteControl && readjustAmountBack != 0)
        {
            lastPosition--;
            readjustAmountBack--;

            // readjustLastPosition = false;
        }

        // Check if already in accepting state initially,
        // if not, compute compute and then re-evaluate upon completion of said computation
        if (currentState == "f")
        {
            isAccepting = true;
            // print the ID
            printInstantaneousDescription(startingPosition, lastPosition, finiteControl, currentState);
        }
        else
        {
            // print the ID
            printInstantaneousDescription(startingPosition, lastPosition, finiteControl, currentState);
            // perform computation
            std::string currentStateAndContent{currentState + *finiteControl};
            // If the key-value pair does not exist, the input crashes the machine, and so we halt
            if (simulator.transitionFunction.count(currentStateAndContent) == 0)
                isHalting = true;

            if (!isHalting)
            {
                std::string move = simulator.transitionFunction[currentStateAndContent];

                currentState = move[0];
                *finiteControl = move[1];
                if (move[2] == 'L')
                {
                    direction = "L";
                    finiteControl--;
                    position--;
                }
                else
                {
                    direction = "R";
                    finiteControl++;
                    position++;
                }
            }
        }
    }

    if (isAccepting && !isHalting)
        std::cout << "Your input was accepted" << std::endl;
    else if (!isAccepting && isHalting)
        std::cout << "Your input was rejected" << std::endl;

    // simulator.display();

    return 0;
}

bool contains(const std::vector<std::string> container, const std::string &letter_val)
{
    for (const auto &letter : container)
        if (letter == letter_val)
            return true;
    return false;
}

void TuringMachine::display()
{
    std::cout << "THE STATES ARE: " << std::endl;
    for (int i{0}; i < states.size(); i++)
    {
        std::cout << '\t' << states[i] << std::endl;
    }

    std::cout << std::endl;

    std::cout << "THE TAPE SYMBOLS ARE: " << std::endl;
    for (int i{0}; i < tapeSymbols.size(); i++)
    {
        std::cout << '\t' << tapeSymbols[i] << std::endl;
    }

    std::cout << std::endl;

    for (auto it{transitionFunction.begin()}; it != transitionFunction.end(); it++)
    {
        std::cout << "Key-Value Pair:" << std::endl;
        std::cout << '\t' << "Key:" << it->first;
        std::cout << '\t' << "Value:" << it->second;
        std::cout << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Number of transition functions: " << transitionFunction.size() << std::endl;

    std::cout << std::endl;

    std::cout << "THE CURRENT SYMBOLS ON THE TAPE: " << std::endl;

    for (auto it{tape.begin()}; it != tape.end(); it++)
    {
        std::cout << *it << " ";
    }
}

void printInstantaneousDescription(std::list<std::string>::iterator start, std::list<std::string>::iterator end, std::list<std::string>::iterator curr, std::string currentState)
{
    while (start != end)
    {
        if (start == curr)
        {
            std::cout << "[" << currentState << "]";
        }
        std::cout << *start;
        start++;
    }

    std::cout << std::endl;
}