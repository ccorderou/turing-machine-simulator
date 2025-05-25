# Turing Machine Simualtor

This project attempts to encapsulate the essence of algorithms by simulating a two-way infinite, one-track, one tape deterministic Turing Machine. I employ C++ for this simulator to ensure fast, reliable, and functional output.

## How it works

This simulator relies on "transition functions" to determine its output. These functions serve as the rules for the language of machine. In other words, if I only want my machine to accept strings with the equal number of 0s followed by an equal number of 1s, such as 0^n1^n, then I need to provide the text file with said rules. Some text files are provided as examples, however users may develop files of their own. Ensure that the file, however, follows the appropriate structure and order expected - simply follow the format from the given files. Once provided, users may enter some string sequence which will be either accepted or rejected by halting or rejected by infinite loop. For the sake of understanding how the machine came to either one of these conclusions, the instantaneous description, or the state the string in each transition, is shown to the user. This visual aid provides an understanding as to why an input was accepted or not.


## What to expect

Once a file with the desired transition functions is provided, the user will input some string of their choice. It may be empty or as long as the user sees fit. Consequently, one of three outputs will result:
- Acceptance
    - The string provided by the user follows the transition functions outlined in the file and is thus recognized by the machine.
- Rejection By Halting
    - The string provided by the user contradicts the rules detailed in the file provided and is therefore rejected by the machine.
- Rejection By Infinite Loop
    - The string provided by the user does not result in a halt and will loop forever. In this case, the machine does not recognize the string nor will it stop to reject it. As such, the machine rejects via infinite loop, a result stemming from how the transition functions are set up.

## Installation Process

Clone this repository:

```bash
git clone https://github.com/ccorderou/turing-machine-simulator.git
cd turing-machine-simulator
```

Once inside the directory, simply execute the program and input any text file, custom-made or provided by this repository. The following instructions utilizes one of the text files for the sake of demonstration of use:

First, compile the program and run the executable file

```bash
g++ -o run turingMachine.cpp
./run
```

Doing so will make the following prompt appear:
```bash
Enter file name: 
```

Here, I will input one of the file text files and will be greeted with the following prompt:
```bash
Enter file name: 0n1n0n.txt
Enter an input word.
Must consist of 0s and/or 1s or none.
Press Ctrl+C to exit the program anytime:
```

Finally, I will type in a valid string to be accepted by the machine. The state of the string in each iteration is shown to provide the user with a visual of how their string came to be accepted in this case:
```bash
Enter a file name: 0n1n0n.txt
Enter an input word.
Must consist of 0s and/or 1s or none.
Press Ctrl+C to exit the program anytime: 001100
[0]001100
X[1]01100
X0[1]1100
X0X[2]100
X0X1[2]00
X0X1X[3]0
X0X1X0[3]B
X0X1X[4]0
X0X1[5]X0
X0X[5]1X0
X0[5]X1X0
X[5]0X1X0
[5]X0X1X0
[5]BX0X1X0
[0]X0X1X0
X[0]0X1X0
XX[1]X1X0
XXX[1]1X0
XXXX[2]X0
XXXXX[2]0
XXXXXX[3]B
XXXXX[4]X
XXXX[4]XX
XXX[4]XXX
XX[4]XXXX
X[4]XXXXX
[4]XXXXXX
[4]BXXXXXX
[f]BBXXXXXX
Your input was: accepted
Enter an input word.
Must consist of 0s and/or 1s or none.
Press Ctrl+C to exit the program anytime:
```
Pressing CTRL+C will exit the program as needed. That's all! I hope you all enjoy!
