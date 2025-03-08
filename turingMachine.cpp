#include <iostream>
#include <fstream>

int main(){

    //Prompt user for file name
    std::cout << "Enter a file name: ";
    string fileName{};
    std::getline(cin, fileName);
    //Instantiate file-reading object
    std::ifstream inputFile(fileName);
     
    return 0;
}