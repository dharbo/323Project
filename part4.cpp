//-------------------------------------------------------------------------------------------------------------
// Group names: Albalas, Emad and Harboyan, David
// Assignment:  Final Project
// Due date:    05/11/2023
// Purpose:     This program translates the program from finalp2.txt to its C++ equivalent. The new
//              file is called part4-output.cpp.
//-------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {

    ifstream file("finalp2.txt"); // File to read.
    ofstream newfile("part4-output.cpp"); // File to write.

    string line;
    vector<string> lines;

    // Store lines from read file into a vector.
    while (getline(file, line))
    {
        lines.push_back(line);
    }

    // Needed statements
    newfile << "#include <iostream>\n";
    newfile << "using namespace std;\n";
    newfile << "int main()\n";

    // Loop through each line.
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].find("program") != string::npos) {
            continue;
        } else if (lines[i].find("var") != string::npos) {
            newfile << "{\n";
            continue;
        } else if (lines[i].find("begin") != string::npos) {
            continue;
        } else if (lines[i].find("end.") != string::npos) {
            newfile << "\treturn 0;\n";
            newfile << "}";
            continue; // should end the loop
        } else if (lines[i].find("integer") != string::npos) { // When declaring variables.
            newfile << "\tint";
            stringstream s1(lines[i]); // To count commas.
            stringstream s2(lines[i]); // Go by token.
            string s;
            int comma_count = 0;

            // Count commas.
            while (getline(s1, s, ' ')) {
                if (s == ",") {
                    comma_count += 1;
                }
            }

            // Write variable declarations to new file.
            while (getline(s2, s, ' ')) {
                if (s != "," && s != "integer" && s != ";" && s != ":") {
                    if (comma_count > 0) {
                        newfile << " " << s << " ,";
                        comma_count -= 1;
                    } else {
                        newfile << " " << s;
                    }
                } else if (s == ";") {
                    newfile << " " << s << "\n";
                }
            }
        } else if (lines[i].find("display") != string::npos) { // When printing output.
            newfile << "\tcout";
            bool params = false;
            stringstream ss(lines[i]); // Go by token.
            string s;

            // Write the display's parameters to new file.
            while (getline(ss, s, ' ')) {
                if (s == "(") {
                    params = true;
                } else if (s == ")") {
                    params = false;
                } else if (params) {
                    if (s != ",") {
                        newfile << " << " << s;
                    }
                } else if (s == ";") {
                    newfile << " << endl " << s << "\n";
                }
            }
        } else { // The rest are already formatted from part 1.
            newfile << "\t" << lines[i] << "\n";
        }

    }
}