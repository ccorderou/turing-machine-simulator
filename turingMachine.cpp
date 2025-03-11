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
    // helper function to reduce redundancies
    bool contains(const std::vector<std::string> container, const std::string &letter);
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

    // if the file does not exist, tell the user to try again
    if (inputFile.fail())
    {
        std::cerr << "Invalid file name. Ensure correct spelling and file location." << std::endl;
        exit(1);
    }

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

    // if the user inputs nothing, request for something
    while (userInput.empty())
    {
        std::cout << "Input must only contain 0s and/or 1s: ";
        std::string userInput{};
        // Use getline to avoid potential cin issues with '\n' when pressing 'Enter' if prompted once more in the future.
        std::getline(std::cin, userInput);
    }

    bool repromptInput{false};
    for (const auto &letter : userInput)
        if (letter != '0' && letter != '1')
            repromptInput = true;

    // if the user inputs symbols not on the alphabet, re-prompt them for the correct input
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

    // An arbitrary amount of blanks are placed in the beginning
    for (int i{0}; i < 5; i++)
        simulator.tape.push_back(blank);

    // Whatever input the user wants, it goes in between the blanks
    for (const auto &letter : userInput)
        simulator.tape.push_back(std::string(1, letter));

    // An arbitrary amount of blanks are placed in the end
    for (int i{0}; i < 5; i++)
        simulator.tape.push_back(blank);

    // The finite control will begin at the start of the tape
    auto finiteControl = simulator.tape.begin();

    // We will readjust it so that it points to the first input seen
    while (*finiteControl == "B")
        finiteControl++;

    // currentState will always be q0, so 0 represents that state
    std::string currentState{"0"};
    // we declare isAccepting & isHalting to be false initially for we do not know if our inputs will accept, reject, or loop infinitely
    bool isAccepting{false};
    bool isHalting{false};

    // the finite control will begin at the first input, so we start there
    auto startingPosition = finiteControl;
    auto lastPosition = finiteControl;

    // our last position will be whenever we see the next blank symbol
    // this means we begin on the first input symbol and end upon the viewing the first blank symbol
    while (*lastPosition != "B")
        lastPosition++;

    // When printing, we would need to adjust our lastPosition according to the input size, and if the
    // read/write head surpressses that length, we need to print that as well
    int readjustAmountBack{0};
    // When printing, our read/write head will begin at the first input. However, if it moves to the left
    // and goes past the first input, we need to capture those moves on our ID, so we will print that as well
    int position{0};
    // Declare a move variable for the transition function
    std::string direction;
    // if our position is past the beginning, set inTheNegatives to be true
    bool inTheNegatives{false};

    auto trueStart = simulator.tape.begin();
    auto trueEnd = simulator.tape.end();
    std::vector<std::string> precedingInputs{};
    std::vector<std::string> trailingInputs{};

    while (!isAccepting && !isHalting)
    {
        // readjust the starting position accordingly so that the appropriate ID is printed
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
        // to avoid an extra blank on the front end to be printed. This situation occurs
        // when position is 0 but it was just a negative number. This condition keeps care of that situation.
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
            std::cout << "what is the bad alloc here#11?" << std::endl;
            // print the ID
            printInstantaneousDescription(startingPosition, lastPosition, finiteControl, currentState);

            std::cout << "what is the bad alloc here#12?" << std::endl;
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
    {
        std::cout << "Your input was: ";
        std::cout << "\033[32m" << "accepted" << "\033[0m" << std::endl;
    }
    else if (!isAccepting && isHalting)
    {
        std::cout << "Your input was: ";
        std::cout << "\033[31m" << "rejected" << "\033[0m" << std::endl;
    }

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