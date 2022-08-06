#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <my_units.h>

using namespace std;
using namespace my_units;

vector<string> tokenize(const string& in) {
    vector<string> tokens;
    size_t start = 0;
    bool reading = false;
    for (size_t pos = 0; pos < in.size(); ++pos) {
        bool space = isspace(in[pos]);
        if (space && reading) {
            // End of token
            string token = in.substr(start, pos);
            tokens.push_back(token);
            reading = false;
        }
        else if (!space && !reading) {
            // Start of token
            start = pos;
            reading = true;
        }
    }
    if (reading) {
        // Push incomplete token
        string token = in.substr(start);
        tokens.push_back(token);
    }
    return tokens;
}

bool parse_value(const string& value_string, double& parsed_value) {
    try {
        parsed_value = stod(value_string);
    } catch (const exception&) {
        return false;
    }
    return true;
}

int main() {
    cout << "Welcome to the units calculator!\n\n"
              << "Available commands:\n"
              << "  R2D <value> - Radians to Degrees\n"
              << "  D2R <value> - Degrees to Radians\n"
              << "  F2C <value> - Fahrenheit to Celcius\n"
              << "  C2F <value> - Celcius to Fahrenheit\n"
              << "  EXIT        - Exit the application\n\n";

    string command_line;

    while (true) {
        cout << ">> ";
        getline(cin, command_line);
        transform(command_line.begin(), command_line.end(), command_line.begin(),
                  [](unsigned char c){ return std::toupper(c); });
        vector<string> tokens = tokenize(command_line);

        if (tokens.size() == 0) {
            continue;
        }

        if (tokens[0] == "EXIT") {
            cout << "Exiting." << endl;
            return 0;
        }

        if (tokens.size() != 2) {
            cout << "Invalid command" << endl;
            continue;
        }

        double value;
        if (!parse_value(tokens[1], value)) {
            cout << "Invalid value." << endl;
            continue;
        }

        if (tokens[0] == "R2D") {
            cout << value << " radians = " << radians_to_degrees(value) << " degrees." << endl;
        } else if (tokens[0] == "D2R") {
            cout << value << " degrees = " << degrees_to_radians(value) << " radians." << endl;
        } else if (tokens[0] == "C2F") {
            cout << value << " celcius = " << celcius_to_fahrenheit(value) << " fahrenheit." << endl;
        } else if (tokens[0] == "F2C") {
            cout << value << " fahrenheit = " << fahrenheit_to_celcius(value) << " celcius." << endl;
        } else {
            cout << "Invalid command." << endl;
        }
    }
}