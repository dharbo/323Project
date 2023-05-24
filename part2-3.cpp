//-------------------------------------------------------------------------------------------------------------
// Group names: Albalas, Emad and Harboyan, David
// Assignment:  Final Project
// Due date:    05/11/2023
// Purpose:     This program accepts or rejects the program from finalp2.txt based on the given
//              grammar. It also detects errors and displays corresponding error messages.
//-------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <map>

using namespace std;

// create parse table
// 0 = blank
// 1 = program I; var B begin H end.
// 2 = LZ
// 3 = DZ
// 4 = C:G;
// 5 = IM
// 6 = ,IM
// 7 = integer
// 8 = JY
// 10 = K
// 11 = S
// 12 = display(VI);
// 13 = "value=",
// 14 = I=E;
// 15 = TQ
// 16 = +TQ
// 17 = -TQ
// 18 = FR
// 19 = *FR
// 20 = /FR
// 21 = (E)
// 22 = I
// 23 = N
// 24 = UDX
// 25 = DX
// 26 = +
// 27 = -
// 28 = 0
// 29 = 1
// 30 = 2
// 31 = 3
// 32 = 4
// 33 = 5
// 34 = 6
// 35 = 7
// 36 = 8
// 37 = 9
// 38 = p
// 39 = q
// 40 = r
// 41 = s
// 100 = Lambda
// 101 = missing ;

int parseTable[23][32] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 101, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 100, 100, 100, 100, 100, 100, 100, 100, 0, 100, 0, 0, 101, 0, 101, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 100, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 100, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 14, 14, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 0, 0, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 16, 17, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0},
    {18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0, 0, 0, 0, 0, 0, 18, 18, 18, 18, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0, 19, 20, 100, 100, 0, 100, 0, 0, 101, 0, 0, 0, 0, 0},
    {23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 22, 22, 22, 22, 0, 0, 0, 0, 0, 0, 23, 23, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 101, 101, 101, 101, 100, 0, 0, 0, 100, 100, 100, 100, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0},
    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 26, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 39, 40, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    
};

// map for to get column and row
map<string, int> columnMap = {
    {"program", 30}, {"\"value=\"", 31}, {"0", 0}, {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9}
    , {"p", 10}, {"q", 11}, {"r", 12}, {"s", 13}, {";", 14}, {",", 15}, {":", 16}, {"=", 17}, {"*", 18}, {"/", 19}, {"+", 20}, {"-", 21}
    , {"(", 22}, {")", 23}, {"$", 24}, {"begin", 25}, {"display", 26}, {"end.", 27}, {"var", 28}, {"integer", 29} 
};
map<char, int> columnCharMap = {
    {'"', 31}, {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}
    , {'p', 10}, {'q', 11}, {'r', 12}, {'s', 13}, {';', 14}, {',', 15}, {':', 16}, {'=', 17}, {'*', 18}, {'/', 19}, {'+', 20}, {'-', 21}
    , {'(', 22}, {')', 23}, {'$', 24} 
};
map<string, int> rowMap ={
    {"P", 0}, {"I", 1}, {"Z", 2}, {"B", 3}, {"C", 4}, {"M", 5}, {"G", 6}, {"H", 7}, {"Y", 8}, {"J", 9}, {"K", 10}, {"V", 11}, {"S", 12}, 
    {"E", 13}, {"Q", 14}, {"T", 15}, {"R", 16}, {"F", 17}, {"N", 18}, {"X", 19}, {"U", 20}, {"D", 21}, {"L", 22}
};

// functions
void charMatch(string currentState, char currentRead);
void printMatch(string currentState, string currentRead);
void printCharMatch(string currentState, char currentRead);
void match(string currentState, string currentRead);

// variables
vector<string> prog;
vector<string> reservedWords = {"program", "var", "display", "integer", "begin", "end.", "\"value=\""};
vector<string> variablesList = {};
string token;
string currentState, currentRead;
int state, terminal, pushItems;
stack<string> currentStack;

int main() {
    // ask user for input and store in variable
    ifstream file("finalp2.txt");
    while (file >> token) {
        prog.push_back(token);
    }
    file.close();

    // check for end.
    if (prog[prog.size() - 1] != "end.") {
        if (prog[prog.size() - 1].size() > 4 || prog[prog.size() - 1].size() < 3) {
            cout << "end. is expected" << endl;
            return 0;
        }
        auto find = prog[prog.size() - 1].find("end");
        if (find != string::npos) {
            if (find == 0 && prog[prog.size() - 1].size() == 3) {
                cout << ". is missing" << endl;
                return 0;
            }
        }
        cout << "end. is expected\n";
        return 0;
    }

    // error checking for reserved words - check if reserved word is present and spelled correctly
    bool isVar = false;
    for (int i = 0; i < reservedWords.size(); i++){
        bool reservedFound = false;
        for (int j = 0; j < prog.size(); j++) {
            if (reservedWords[i] == prog[j]) {
                reservedFound = true;
            }
        }
        if (!reservedFound && reservedWords[i] != "\"value=\"" && reservedWords[i] != "display") {
            cout << reservedWords[i] << " is expected" << endl;
            return 0;
        }
    }

    // capture all defined variables in a list
    for (int i = 0; i < prog.size(); i++) {
        if (prog[i] == "var") {
                isVar = true;
            }
        if (prog[i] == ":") {
                isVar = false;
            }
        if (isVar && prog[i] != "var" && prog[i] != ",") {
            variablesList.push_back(prog[i]);
            int next = i + 1;
            if (prog[next] != "," && prog[next] != ":") {
                if (prog[next] == "integer") {
                    cout << ": is missing!" << endl;
                    return 0;
                }
                cout << ", is missing!" << endl;
                return 0;
            }
        }

    }

    // push $ and starting state P
    currentStack.push("$");
    currentStack.push("P");

    // begin flag
    bool hasBegin = false;
    // loop through the every item in vector
    for (int i = 0; i < prog.size(); i++) {
        // store item in read variable
        currentRead = prog[i];
        // reserved word flag set to false
        bool isRev = false;
        // is read variable a reserved word?
        for (int i = 0; i < reservedWords.size(); i++) {
            if (currentRead == reservedWords[i]) {
                isRev = true;
            }
        }

        // catch unknown vars
        // start once we read begin reserved word and set begin flag to true
        if (currentRead == "begin") {
            hasBegin = true;
        }
        // set begin flag to false once we read end. reserved word
        if (currentRead == "end.") {
            hasBegin = false;
        }
        // catch known varaibles in a vector
        if (hasBegin && !isRev) {
            // known variable flag
            bool knownVar = true;
            // create variable that holds top of stack
            string stackItem = currentStack.top();
            if (stackItem == "H" || stackItem == "Y" || stackItem == "I" || stackItem == "E"|| stackItem == "T" || stackItem == "V") {
                if (currentRead[0] > 65 && currentRead[0] < 122) {
                    for (string v : variablesList) {
                        if (prog[i] != v) {
                            knownVar = false;
                        } else if (prog[i] == v) {
                            knownVar = true;
                            break;
                        }
                }
                }
                if (!knownVar) {
                    int next = i + 1;
                    if (currentRead.size() > 1 && prog[next] == "(") {
                        cout << "display is expected!" << endl;
                        return 0;
                    }
                    cout << "Not accepted!\nUnknown variable: " << currentRead << endl;
                    return 0;
                }
            }
        }

        // if read is not a reserved word then go char by char
        if (!isRev) {
            // break string out and go through table
            for (int i = 0; i < currentRead.size(); i++) {
                char currRead = currentRead.at(i);
                currentState = currentStack.top();
                currentStack.pop();
                charMatch(currentState, currRead);
                // check if rejected and end program
                if (pushItems == 0 || pushItems == 101) {
                    return 0;
                }
            }
            continue;
        }


        // store current currentState and then pop
        currentState = currentStack.top();
        currentStack.pop();
        match(currentState, currentRead);
        // check if its been rejected and end program
        if (pushItems == 0 || pushItems == 101) {
            return 0;
        }
    }  
    return 0;
}

void printMatch(string currentState, string currentRead){
    if (currentState == currentRead && pushItems != 0) {
        cout << "Match found for: " << currentRead << endl;
        stack<string> temp = currentStack;
        int stackSize = currentStack.size();
        cout << "Stack content: ";
        for (int j = 0; j < stackSize; j++) {
            cout << temp.top() << " ";
            temp.pop();
            if (j == stackSize - 1) {
                cout << "\n" << endl;
            }
        }
    }
    if (currentStack.top() == "$" && pushItems == 100) {
        cout << "Accepted!" << endl;
    }
};

void printCharMatch(string currentState, char currentRead){
    if (currentState[0] == currentRead) {
                cout << "Match found for: " << currentRead << endl;
                stack<string> temp = currentStack;
                int stackSize = currentStack.size();
                cout << "Stack content: ";
                for (int j = 0; j < stackSize; j++) {
                    cout << temp.top() << " ";
                    temp.pop();
                    if (j == stackSize - 1) {
                        cout << "\n" << endl;
                    }
                }
            }
};

void match(string currentState, string currentRead) {
    printMatch(currentState, currentRead);
    while (currentRead != currentState && pushItems != 101) {
            terminal = columnMap[currentRead];
            state = rowMap[currentState];
            // store value from table using the state and terminal and perform necessary actions
            pushItems = parseTable[state][terminal];
            switch (pushItems) {
                // if user input is rejected
                case 0:
                    cout << "Not accepted!" << endl;
                    if (currentState == ";") {
                        cout << "; is missing!" << endl;
                    }
                    if (currentState == "(" || currentRead == ")") {
                        cout << "( is missing!" << endl;
                    }
                    if (currentState == ")") {
                        cout << ") is missing!" << endl;
                    }
                    break;
                case 1:
                    currentStack.push("end.");
                    currentStack.push("H");
                    currentStack.push("begin");
                    currentStack.push("B");
                    currentStack.push("var");
                    currentStack.push(";");
                    currentStack.push("I");
                    currentStack.push("program");
                    break;
                case 2:
                    currentStack.push("Z");
                    currentStack.push("L");
                    break;
                case 3:
                    currentStack.push("Z");
                    currentStack.push("D");
                    break;
                case 4:
                    currentStack.push(";");
                    currentStack.push("G");
                    currentStack.push(":");
                    currentStack.push("C");
                    break;
                case 5:
                    currentStack.push("M");
                    currentStack.push("I");
                    break;
                case 6:
                    currentStack.push("M");
                    currentStack.push("I");
                    currentStack.push(",");
                    break;
                case 7:
                    currentStack.push("integer");
                    break;
                case 8:
                    currentStack.push("Y");
                    currentStack.push("J");
                    break;
                case 9:
                    currentStack.push("J");
                    break;
                case 10:
                    currentStack.push("K");
                    break;
                case 11:
                    currentStack.push("S");
                    break;
                case 12:
                    currentStack.push(";");
                    currentStack.push(")");
                    currentStack.push("I");
                    currentStack.push("V");
                    currentStack.push("(");
                    currentStack.push("display");
                    break;
                case 13:
                    currentStack.push(",");
                    currentStack.push("\"value=\"");
                    break;
                case 14:
                    currentStack.push(";");
                    currentStack.push("E");
                    currentStack.push("=");
                    currentStack.push("I");
                    break;
                case 15:
                    currentStack.push("Q");
                    currentStack.push("T");
                    break;
                case 16:
                    currentStack.push("Q");
                    currentStack.push("T");
                    currentStack.push("+");
                    break;
                case 17:
                    currentStack.push("Q");
                    currentStack.push("T");
                    currentStack.push("-");
                    break;
                case 18:
                    currentStack.push("R");
                    currentStack.push("F");
                    break;
                case 19:
                    currentStack.push("R");
                    currentStack.push("F");
                    currentStack.push("*");
                    break;
                case 20:
                    currentStack.push("R");
                    currentStack.push("F");
                    currentStack.push("/");
                    break;
                case 21:
                    currentStack.push(")");
                    currentStack.push("E");
                    currentStack.push("(");
                    break;
                case 22:
                    currentStack.push("I");
                    break;
                case 23:
                    currentStack.push("N");
                    break;
                case 24:
                    currentStack.push("X");
                    currentStack.push("D");
                    currentStack.push("U");
                    break;
                case 25:
                    currentStack.push("X");
                    currentStack.push("D");
                    break;
                case 26:
                    currentStack.push("+");
                    break;
                case 27:
                    currentStack.push("-");
                    break;
                case 28:
                    currentStack.push("0");
                    break;
                case 29:
                    currentStack.push("1");
                    break;
                case 30:
                    currentStack.push("2");
                    break;
                case 31:
                    currentStack.push("3");
                    break;
                case 32:
                    currentStack.push("4");
                    break;
                case 33:
                    currentStack.push("5");
                    break;
                case 34:
                    currentStack.push("6");
                    break;
                case 35:
                    currentStack.push("7");
                    break;
                case 36:
                    currentStack.push("8");
                    break;
                case 37:
                    currentStack.push("9");
                    break;
                case 38:
                    currentStack.push("p");
                    break;
                case 39:
                    currentStack.push("q");
                    break;
                case 40:
                    currentStack.push("r");
                    break;
                case 41:
                    currentStack.push("s");
                    break;
                case 100:
                    break;
                case 101:
                    cout << "Not accepted!" << endl;
                    cout << "; is missing!" << endl;
                    break;
            }
            if (pushItems == 0) {break;}
            // grab new current state and pop
            currentState = currentStack.top();
            currentStack.pop();
            printMatch(currentState, currentRead);
        }
};

void charMatch(string currentState, char currentRead) {
    printCharMatch(currentState, currentRead);
    while(currentState[0] != currentRead && pushItems != 101){
            terminal = columnCharMap[currentRead];
            state = rowMap[currentState];
            // store value from table using the state and terminal and perform necessary actions
            pushItems = parseTable[state][terminal];
            switch (pushItems) {
                // if user input is rejected
                case 0:
                    cout << "Not accepted!" << endl;
                    if (currentState == ",") {
                        cout << ", is missing!" << endl;
                    }
                    if (currentState == ";" && currentRead != ')') {
                        cout << "; is missing!" << endl;
                    }
                    if (currentState == "(" || currentRead == ')') {
                        cout << "( is missing!" << endl;
                    }
                    if (currentState == ")") {
                        cout << ") is missing!" << endl;
                    }
                    return;
                case 1:
                    currentStack.push("end.");
                    currentStack.push("H");
                    currentStack.push("begin");
                    currentStack.push("B");
                    currentStack.push("var");
                    currentStack.push(";");
                    currentStack.push("I");
                    currentStack.push("program");
                    break;
                case 2:
                    currentStack.push("Z");
                    currentStack.push("L");
                    break;
                case 3:
                    currentStack.push("Z");
                    currentStack.push("D");
                    break;
                case 4:
                    currentStack.push(";");
                    currentStack.push("G");
                    currentStack.push(":");
                    currentStack.push("C");
                    break;
                case 5:
                    currentStack.push("M");
                    currentStack.push("I");
                    break;
                case 6:
                    currentStack.push("M");
                    currentStack.push("I");
                    currentStack.push(",");
                    break;
                case 7:
                    currentStack.push("integer");
                    break;
                case 8:
                    currentStack.push("Y");
                    currentStack.push("J");
                    break;
                case 9:
                    currentStack.push("J");
                    break;
                case 10:
                    currentStack.push("K");
                    break;
                case 11:
                    currentStack.push("S");
                    break;
                case 12:
                    currentStack.push(";");
                    currentStack.push(")");
                    currentStack.push("I");
                    currentStack.push("V");
                    currentStack.push("(");
                    currentStack.push("display");
                    break;
                case 13:
                    currentStack.push(",");
                    currentStack.push("\"value=\"");
                    break;
                case 14:
                    currentStack.push(";");
                    currentStack.push("E");
                    currentStack.push("=");
                    currentStack.push("I");
                    break;
                case 15:
                    currentStack.push("Q");
                    currentStack.push("T");
                    break;
                case 16:
                    currentStack.push("Q");
                    currentStack.push("T");
                    currentStack.push("+");
                    break;
                case 17:
                    currentStack.push("Q");
                    currentStack.push("T");
                    currentStack.push("-");
                    break;
                case 18:
                    currentStack.push("R");
                    currentStack.push("F");
                    break;
                case 19:
                    currentStack.push("R");
                    currentStack.push("F");
                    currentStack.push("*");
                    break;
                case 20:
                    currentStack.push("R");
                    currentStack.push("F");
                    currentStack.push("/");
                    break;
                case 21:
                    currentStack.push(")");
                    currentStack.push("E");
                    currentStack.push("(");
                    break;
                case 22:
                    currentStack.push("I");
                    break;
                case 23:
                    currentStack.push("N");
                    break;
                case 24:
                    currentStack.push("X");
                    currentStack.push("D");
                    currentStack.push("U");
                    break;
                case 25:
                    currentStack.push("X");
                    currentStack.push("D");
                    break;
                case 26:
                    currentStack.push("+");
                    break;
                case 27:
                    currentStack.push("-");
                    break;
                case 28:
                    currentStack.push("0");
                    break;
                case 29:
                    currentStack.push("1");
                    break;
                case 30:
                    currentStack.push("2");
                    break;
                case 31:
                    currentStack.push("3");
                    break;
                case 32:
                    currentStack.push("4");
                    break;
                case 33:
                    currentStack.push("5");
                    break;
                case 34:
                    currentStack.push("6");
                    break;
                case 35:
                    currentStack.push("7");
                    break;
                case 36:
                    currentStack.push("8");
                    break;
                case 37:
                    currentStack.push("9");
                    break;
                case 38:
                    currentStack.push("p");
                    break;
                case 39:
                    currentStack.push("q");
                    break;
                case 40:
                    currentStack.push("r");
                    break;
                case 41:
                    currentStack.push("s");
                    break;
                case 100:
                    break;
                case 101:
                    cout << "Not accepted!" << endl;
                    cout << "; is missing!" << endl;
                    break;
            }
            // grab new current state and pop
            currentState = currentStack.top();
            currentStack.pop();
            printCharMatch(currentState, currentRead);
    }
}