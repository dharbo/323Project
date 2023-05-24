//-------------------------------------------------------------------------------------------------------------
// Group names: Albalas, Emad and Harboyan, David
// Assignment:  Final Project
// Due date:    05/11/2023
// Purpose:     This program removes comments and blank lines, and extra spaces. It writes the output
//              to a new file called finalp2.txt.
//-------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Return if there is a multiline comment.
vector<int> Multiline(vector<string> lines, int i, int startIndex) {
    int startingLine = i;
    for (; i < lines.size(); ++i) {
        string line = lines[i];
        if (startingLine == i) { // Same line comment.
            auto endIndex = line.rfind("//");
            if (endIndex != string::npos && endIndex != startIndex && endIndex == line.length() - 2) { // Single comment.
                return {2}; // Means a single line comment that starts and ends on the same line
            }
        } else if (startingLine != i) { // Different line comment.
            auto endIndex = line.find("//");
            if (endIndex != string::npos && endIndex == line.length() - 2) { // Multiline comment. Return true, index of vector, index of line.
                return {1, i, (int)endIndex}; // endIndex might not be needed.
            } else if (endIndex != string::npos && endIndex != line.length() - 2) { // Not a multiline comment.
                return {0}; // Return 0, meaning not a multiline comment.
            }
        }
    }
    return {0}; // Default value
}

int main()
{
    // Read and store tokens from h5.txt.
    ifstream file("finalp1.txt");
    ofstream newfile("finalp2.txt");

    vector<string> lines;
    vector<string> commentsRemoved;

    string line;

    // Remove any leading or trailing whitespace.
    while (getline(file, line))
    {
        if (line.empty()) {
            continue;
        }
        string result = "";
        const auto strBegin = line.find_first_not_of(" ");
        if (strBegin != string::npos)
        {
            const auto strEnd = line.find_last_not_of(" ");
            const auto strRange = strEnd - strBegin + 1;

            result = line.substr(strBegin, strRange);
        }

        // Replace sub ranges.
        auto beginSpace = result.find_first_of(" ");
        while (beginSpace != string::npos)
        {
            const auto endSpace = result.find_first_not_of(" ", beginSpace);
            const auto range = endSpace - beginSpace;

            result.replace(beginSpace, range, " ");

            const auto newStart = beginSpace + 1;
            beginSpace = result.find_first_of(" ", newStart);
        }
        lines.push_back(result);
    }

    // Remove comments.
    for (int i = 0; i < lines.size(); ++i)
    {
        string line = lines[i];

        auto startIndex = line.find("//");
        if (startIndex == string::npos) { // When no comment.
            commentsRemoved.push_back(line);
        } else if (startIndex != string::npos) { // When there is a comment
            vector<int> result = Multiline(lines, i, startIndex);
            if (result[0] == 0) { // Not multiline comment.
                line = line.substr(0, startIndex);
            } else if (result[0] == 1) { // Multiline comment.
                i = i + (result[1] - i); // If we continue, the loop will inc for us.
                continue;
            } else if (result[0] == 2) { // If comment ends and starts on same line.
                continue;
            }
            if (!line.empty()) {
                commentsRemoved.push_back(line);
            }
        }
    }

    // Add spaces, fix formatting.
    for (int i = 0; i < commentsRemoved.size(); ++i) {
        bool inQuotes = false;
        // Check for some key words.
        if (commentsRemoved[i] == "var" || commentsRemoved[i] == "begin") {
            newfile << commentsRemoved[i] + " \n"; // Write to finalp2.txt.
            continue;
        } else if (commentsRemoved[i] == "end.") {
            newfile << commentsRemoved[i] << " "; // Write to finalp2.txt.
            continue;
        }
        // Loop through each char of each string.
        for (int j = 0; j < commentsRemoved[i].length(); ++j) {
            char ch = commentsRemoved[i][j];

            // Check for quotes.
            if (inQuotes && ch == '\"') {
                inQuotes = false;
            } else if (ch == '\"') {
                inQuotes = true;
            }
            if (inQuotes) {
                continue;
            }

            char prech, postch;

            // Space before operator.
            if (j > 0) {
                prech = commentsRemoved[i][j-1];
            }
            if (prech != ' ' && (ch == ';' || ch == ':' || ch == ',' || ch == '=' || ch == '+' ||
                    ch == '-' || ch == '/' || ch == '*' || ch == '(' || ch == ')')) {
                commentsRemoved[i].insert(j, " ");
                ++j;
            }

            // Space after operator.
            if (j < commentsRemoved[i].length()) {
                postch = commentsRemoved[i][j+1];
            }
            if (postch != ' ' && (ch == ';' || ch == ':' || ch == ',' || ch == '=' || ch == '+' ||
                    ch == '-' || ch == '/' || ch == '*' || ch == '(' || ch == ')')) {
                commentsRemoved[i].insert(j+1, " ");
            }
        }
        // Write to finalp2.txt.
        newfile << commentsRemoved[i] + "\n";
    }

    file.close();
    newfile.close();

    return 0;
}